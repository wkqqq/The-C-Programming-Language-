/****************************************************************
文件功能:闸机开门防夹程序--包括红外防夹和物理防夹
文件版本： 
最新更新:wk20191024添加 M_ZJAntiPinchInOpenDoor.c文件
说明：左开门表示由左边红外或左边刷卡进入闸机	
****************************************************************/

#include "M_include.h"
//#include "M_ZJAntiPinchInOpenDoor.h"

/* 文件内部函数声明 */

void JudgeHumanPos(void); //根据红外判断人在闸机的位置
OPEN_DOOR_FLAG_ENUM GetOpeningDoorFlag(void);//是否正在开门判断 1:正在开门

void JudgeOpeningDoorFJ(OPEN_DOOR_FLAG_ENUM dir); //开门防夹判断
void OpeningDoorFJRebound(void); //开门防夹弹回




AVOIDHURT_OPENDOOR_STRUCT OpenDoorAvoidHurt;    //开门防夹结构体

void InitData_OpenDoorAvoidHurt(void)   //开门防夹参数初始化
{
    OpenDoorAvoidHurt.DealingFlag = 0;
    OpenDoorAvoidHurt.DelayCount = 0;
    
    OpenDoorAvoidHurt.OpenDoorFJReboundID = 0;
    OpenDoorAvoidHurt.LeftOpenWuLiFJID = 0;
    OpenDoorAvoidHurt.RightOpenInfraredFJID = 0;
    OpenDoorAvoidHurt.RightOpenWuLiFJID = 0;

//    OpenDoorAvoidHurt.OpeningDoorDir = NOT_OPENING_DOOR;
    
}

/* 
* 功能:开门过程中防夹处理
*
*/
void AvoidHurtInOpenDoor(void)
{
    JudgeHumanPos(); //根据红外判断人在闸机的位置

    JudgeOpeningDoorFJ(GetOpeningDoorFlag());   //开门防夹判断

    if(0 == OpenDoorAvoidHurt.DealingFlag){     //不需要进行防夹处理
        return;
    }
    else{
        
        ZJDI.OpenLeft = 0;    //屏蔽左开门 
        ZJDI.OpenLeftFree = 0;
        ZJDI.OpenRight = 0;    //屏蔽右开门
        ZJDI.OpenRightFree = 0;

        //报警
       // UpdateDataFuncCodeValue(1,5,1, 32);										
       // UpdateDataFuncCodeValue(1,5,3, 1);
    }
    
    OpeningDoorFJRebound(); //开门防夹弹回

}




/*
* 功能:判断闸机是否正在开门 0:未处于开门中 1:左开门中 2:右开门中
*   给到 OpenDoorAvoidHurt.OpeningDoorDir
*/
OPEN_DOOR_FLAG_ENUM GetOpeningDoorFlag(void)
{
    OPEN_DOOR_FLAG_ENUM m_OpeningDoorFlag = NOT_OPENING_DOOR;  //开门状态标志位
    
    _iq m_OutSpeed; //电机速度
    m_OutSpeed = LowFilter_Fdb_speed.Out;

    if(0 != OpenDoorAvoidHurt.DealingFlag){ //正在处理开门防夹，则不需要再进行判断
        return NOT_OPENING_DOOR;
    }
    
    if(m_OutSpeed < 0){ //取当前速度的绝对值
        m_OutSpeed = -m_OutSpeed;
    }
    if((ZJCOMMON.bit.SON == 1) && (ZJCOMMON.bit.MUL == 1) && (m_OutSpeed > _IQ(0.007))){ //开门中且闸机速度大于21rpm
        if(ZJCOMMON.bit.CMD == 1){    //左开门
            m_OpeningDoorFlag = OPENING_LEFT_DOOR;
        }
        else if(ZJCOMMON.bit.CMD == 2){  //右开门
            m_OpeningDoorFlag = OPENING_RIGHT_DOOR;
        }
        else{
            m_OpeningDoorFlag = NOT_OPENING_DOOR;
        }
    }

    return m_OpeningDoorFlag;
    
}





/*
* 功能:开门防夹触发后弹回--重新关门一次
* 红外防夹和物理防夹的弹回都使用本函数
*/
void OpeningDoorFJRebound(void) //开门防夹弹回
{
//左右开门防夹同时处理
    switch(OpenDoorAvoidHurt.OpenDoorFJReboundID){
        
        case 1: //第一步,暂停运动,同时报警
    
            ZJCOMMON.bit.HOLD = 1;  //暂停
            ZJCOMMON.bit.MUL = 0;
        
            ZJLH.LockEnable = 0;    //屏蔽离合，防止锁离合的咔嚓声          
            
            limit.iq_ccw = ZhaJi.FJiq;//_IQ(0.15);//限制转矩
            limit.iq_cw = ZhaJi.FJiq;//_IQ(0.15);
            
            OpenDoorAvoidHurt.OpenDoorFJReboundID = 2; //第二步
            break;
            
		case 2: //第二步,等待停止后,取消停止状态
            if((ZJCOMMON.all == ZJCOMMONFUJI.all) //控制信号同步完成
            &&(LowFilter_Fdb_speed.Out<_IQ(0.005)) && (LowFilter_Fdb_speed.Out>-_IQ(0.005))) //且速度降下来
			{
                ZJCOMMON.bit.HOLD = 0;  //取消暂停并停掉电机
                ZJCOMMON.bit.SON = 0;

                if(++OpenDoorAvoidHurt.DelayCount > 200){    //延时40ms再次屏蔽离合
                
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 3;
                    OpenDoorAvoidHurt.DelayCount = 0;
                    LockDisableCtrl();
                }
			}				
            break;
            
		case 3: //第三步 重新开动电机并关门
		    if( ((1 == OpenDoorAvoidHurt.DealingFlag) && (2 != OpenDoorAvoidHurt.LeftHumanPos)) //左开门后可以关门
                || ((2 == OpenDoorAvoidHurt.DealingFlag) && (2 != OpenDoorAvoidHurt.RightHumanPos))) //右开门后可以关门
            {      
    			ZJCOMMON.bit.SON = 1; //重新开机并关门
    			ZJCOMMON.bit.MUL = 1;
    			ZJCOMMON.bit.CMD = 0;
                
    			//GreenLedDLtoR();    //顶灯显示红叉
    			limit.iq_ccw = limit.iq_ccwlast;//恢复转矩
    			limit.iq_cw = limit.iq_cwlast;

    			OpenDoorAvoidHurt.OpenDoorFJReboundID = 4;
            }
            break;
            
        case 4: //第四步，等待开门对侧的人退出通道

            if((ZJCOMMON.all == ZJCOMMONFUJI.all) && (0 == DRCommRcvReg.bit.CMD))   //等待控制字传输完毕
            
			{
			    if((0 == ZJDI.InfraredRight1Last) && (0 == ZJDI.InfraredRight1) && (1 == OpenDoorAvoidHurt.DealingFlag)){   //等待对侧人退出通道--此时闸机正在关门
				    OpenDoorAvoidHurt.OpenDoorFJReboundID = 5;
                 }

                if((0 == ZJDI.InfraredLeft1Last) && (0 == ZJDI.InfraredLeft1) && (2 == OpenDoorAvoidHurt.DealingFlag)){
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 6;
                }
			}
            break;
            
		case 5: //第五步，继续等待关门完成
			if(++OpenDoorAvoidHurt.DelayCount > 40)
			{
				OpenDoorAvoidHurt.DelayCount = 41;
				if(gSendToMotorDriveStatus.bit.multiPosEND && ZJCOMMONFUJI.bit.SEND)
				{
					OpenDoorAvoidHurt.DelayCount = 0;
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 0;
                    OpenDoorAvoidHurt.DealingFlag = 0;
                    ZhaJiLeftMODE_ID = 0;
                    ZJDI.OpenLeft=1;    //重新左开门
                    ZJLH.LockEnable = GetFuncCodeValue(1,4,29); //恢复离合使能位
                    
					//OpenDoorAvoidHurt.OpenDoorFJReboundID = 6;							
				}
			}
            break;

        case 6: //第五步，继续等待关门完成
			if(++OpenDoorAvoidHurt.DelayCount > 40)
			{
				OpenDoorAvoidHurt.DelayCount = 41;
				if(gSendToMotorDriveStatus.bit.multiPosEND && ZJCOMMONFUJI.bit.SEND)
				{
					OpenDoorAvoidHurt.DelayCount = 0;
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 0;
                    OpenDoorAvoidHurt.DealingFlag = 0;
                    ZhaJiLeftMODE_ID = 0;
                    ZJDI.OpenRight=1;    //重新右开门
                    ZJLH.LockEnable = GetFuncCodeValue(1,4,29); //恢复离合使能位
                    
					//OpenDoorAvoidHurt.OpenDoorFJReboundID = 6;							
				}
			}
            break;
            
        default:
            break;
            

    }    
}




/*
* 功能:开门防夹判断
* 
*/
void JudgeOpeningDoorFJ(OPEN_DOOR_FLAG_ENUM dir) //开门防夹判断
{
    if(NOT_OPENING_DOOR == dir){    //没有在开门
        return;
    }

    if(0 != OpenDoorAvoidHurt.DealingFlag){ //已经在进行防夹处理中
        return;
    }

// 红外防夹判断    
    if(((1 == ZJDI.InfraredRight1Last) && (1 == ZJDI.InfraredRight1) && (OPENING_LEFT_DOOR == dir)))   //左开门时右1红外有信号,暂不考虑防夹红外
       // || (((1 == ZhaJi.MODE_ID) || (3 == ZhaJi.MODE_ID) || (4 == ZhaJi.MODE_ID)) && (1 == ZJDI.InfraredFangJia))) //左刷卡模式下，仅仅只有防夹信号也会触发开门防夹
    {
        OpenDoorAvoidHurt.OpenDoorFJReboundID = 1;
        OpenDoorAvoidHurt.DealingFlag = 1;
        ZhaJiLeftMODE_ID = 77; //不为0且在default里的值，屏蔽辅机离合
        CLOSEDOORMODE_ID = 0;
        return;
    }

    if(((1 == ZJDI.InfraredLeft1Last) && (1 == ZJDI.InfraredLeft1) && (OPENING_RIGHT_DOOR == dir)))   //右开门时左1红外有信号,暂不考虑防夹红外
    {
        OpenDoorAvoidHurt.OpenDoorFJReboundID = 1;
        OpenDoorAvoidHurt.DealingFlag = 2;
        ZhaJiLeftMODE_ID = 77;
        CLOSEDOORMODE_ID = 0;
        return;
    }

// 物理防夹判断

    /* 待添加 */

    
}



/*
* 功能:根据红外判断人在闸机的具体位置
* 
*/
void JudgeHumanPos(void)
{
    if((0 == ZJDI.InfraredLeft1) && (0 == ZJDI.InfraredFangJia)){ //左侧闸机内无人
        OpenDoorAvoidHurt.LeftHumanPos = 0;
    }
    else if((1 == ZJDI.InfraredLeft1) && (0 == ZJDI.InfraredFangJia)){ //左侧闸机有人但对侧关门不会被夹到
        OpenDoorAvoidHurt.LeftHumanPos = 1;
    }
    else{
        OpenDoorAvoidHurt.LeftHumanPos = 2;
    }


    if((0 == ZJDI.InfraredRight1) && (0 == ZJDI.InfraredFangJia)){ //右侧闸机内无人
        OpenDoorAvoidHurt.RightHumanPos = 0;
    }
    else if((1 == ZJDI.InfraredRight1) && (0 == ZJDI.InfraredFangJia)){ //右侧闸机有人但对侧关门不会被夹到
        OpenDoorAvoidHurt.RightHumanPos = 1;
    }
    else{
        OpenDoorAvoidHurt.RightHumanPos = 2;
    }
    
}





