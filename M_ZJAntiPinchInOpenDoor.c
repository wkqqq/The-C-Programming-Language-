/****************************************************************
�ļ�����:բ�����ŷ��г���--����������к��������
�ļ��汾�� 
���¸���:wk20191024��� M_ZJAntiPinchInOpenDoor.c�ļ�
˵�������ű�ʾ����ߺ�������ˢ������բ��	
****************************************************************/

#include "M_include.h"
//#include "M_ZJAntiPinchInOpenDoor.h"

/* �ļ��ڲ��������� */

void JudgeHumanPos(void); //���ݺ����ж�����բ����λ��
OPEN_DOOR_FLAG_ENUM GetOpeningDoorFlag(void);//�Ƿ����ڿ����ж� 1:���ڿ���

void JudgeOpeningDoorFJ(OPEN_DOOR_FLAG_ENUM dir); //���ŷ����ж�
void OpeningDoorFJRebound(void); //���ŷ��е���




AVOIDHURT_OPENDOOR_STRUCT OpenDoorAvoidHurt;    //���ŷ��нṹ��

void InitData_OpenDoorAvoidHurt(void)   //���ŷ��в�����ʼ��
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
* ����:���Ź����з��д���
*
*/
void AvoidHurtInOpenDoor(void)
{
    JudgeHumanPos(); //���ݺ����ж�����բ����λ��

    JudgeOpeningDoorFJ(GetOpeningDoorFlag());   //���ŷ����ж�

    if(0 == OpenDoorAvoidHurt.DealingFlag){     //����Ҫ���з��д���
        return;
    }
    else{
        
        ZJDI.OpenLeft = 0;    //�������� 
        ZJDI.OpenLeftFree = 0;
        ZJDI.OpenRight = 0;    //�����ҿ���
        ZJDI.OpenRightFree = 0;

        //����
       // UpdateDataFuncCodeValue(1,5,1, 32);										
       // UpdateDataFuncCodeValue(1,5,3, 1);
    }
    
    OpeningDoorFJRebound(); //���ŷ��е���

}




/*
* ����:�ж�բ���Ƿ����ڿ��� 0:δ���ڿ����� 1:������ 2:�ҿ�����
*   ���� OpenDoorAvoidHurt.OpeningDoorDir
*/
OPEN_DOOR_FLAG_ENUM GetOpeningDoorFlag(void)
{
    OPEN_DOOR_FLAG_ENUM m_OpeningDoorFlag = NOT_OPENING_DOOR;  //����״̬��־λ
    
    _iq m_OutSpeed; //����ٶ�
    m_OutSpeed = LowFilter_Fdb_speed.Out;

    if(0 != OpenDoorAvoidHurt.DealingFlag){ //���ڴ����ŷ��У�����Ҫ�ٽ����ж�
        return NOT_OPENING_DOOR;
    }
    
    if(m_OutSpeed < 0){ //ȡ��ǰ�ٶȵľ���ֵ
        m_OutSpeed = -m_OutSpeed;
    }
    if((ZJCOMMON.bit.SON == 1) && (ZJCOMMON.bit.MUL == 1) && (m_OutSpeed > _IQ(0.007))){ //��������բ���ٶȴ���21rpm
        if(ZJCOMMON.bit.CMD == 1){    //����
            m_OpeningDoorFlag = OPENING_LEFT_DOOR;
        }
        else if(ZJCOMMON.bit.CMD == 2){  //�ҿ���
            m_OpeningDoorFlag = OPENING_RIGHT_DOOR;
        }
        else{
            m_OpeningDoorFlag = NOT_OPENING_DOOR;
        }
    }

    return m_OpeningDoorFlag;
    
}





/*
* ����:���ŷ��д����󵯻�--���¹���һ��
* ������к�������еĵ��ض�ʹ�ñ�����
*/
void OpeningDoorFJRebound(void) //���ŷ��е���
{
//���ҿ��ŷ���ͬʱ����
    switch(OpenDoorAvoidHurt.OpenDoorFJReboundID){
        
        case 1: //��һ��,��ͣ�˶�,ͬʱ����
    
            ZJCOMMON.bit.HOLD = 1;  //��ͣ
            ZJCOMMON.bit.MUL = 0;
        
            ZJLH.LockEnable = 0;    //������ϣ���ֹ����ϵ�������          
            
            limit.iq_ccw = ZhaJi.FJiq;//_IQ(0.15);//����ת��
            limit.iq_cw = ZhaJi.FJiq;//_IQ(0.15);
            
            OpenDoorAvoidHurt.OpenDoorFJReboundID = 2; //�ڶ���
            break;
            
		case 2: //�ڶ���,�ȴ�ֹͣ��,ȡ��ֹͣ״̬
            if((ZJCOMMON.all == ZJCOMMONFUJI.all) //�����ź�ͬ�����
            &&(LowFilter_Fdb_speed.Out<_IQ(0.005)) && (LowFilter_Fdb_speed.Out>-_IQ(0.005))) //���ٶȽ�����
			{
                ZJCOMMON.bit.HOLD = 0;  //ȡ����ͣ��ͣ�����
                ZJCOMMON.bit.SON = 0;

                if(++OpenDoorAvoidHurt.DelayCount > 200){    //��ʱ40ms�ٴ��������
                
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 3;
                    OpenDoorAvoidHurt.DelayCount = 0;
                    LockDisableCtrl();
                }
			}				
            break;
            
		case 3: //������ ���¿������������
		    if( ((1 == OpenDoorAvoidHurt.DealingFlag) && (2 != OpenDoorAvoidHurt.LeftHumanPos)) //���ź���Թ���
                || ((2 == OpenDoorAvoidHurt.DealingFlag) && (2 != OpenDoorAvoidHurt.RightHumanPos))) //�ҿ��ź���Թ���
            {      
    			ZJCOMMON.bit.SON = 1; //���¿���������
    			ZJCOMMON.bit.MUL = 1;
    			ZJCOMMON.bit.CMD = 0;
                
    			//GreenLedDLtoR();    //������ʾ���
    			limit.iq_ccw = limit.iq_ccwlast;//�ָ�ת��
    			limit.iq_cw = limit.iq_cwlast;

    			OpenDoorAvoidHurt.OpenDoorFJReboundID = 4;
            }
            break;
            
        case 4: //���Ĳ����ȴ����ŶԲ�����˳�ͨ��

            if((ZJCOMMON.all == ZJCOMMONFUJI.all) && (0 == DRCommRcvReg.bit.CMD))   //�ȴ������ִ������
            
			{
			    if((0 == ZJDI.InfraredRight1Last) && (0 == ZJDI.InfraredRight1) && (1 == OpenDoorAvoidHurt.DealingFlag)){   //�ȴ��Բ����˳�ͨ��--��ʱբ�����ڹ���
				    OpenDoorAvoidHurt.OpenDoorFJReboundID = 5;
                 }

                if((0 == ZJDI.InfraredLeft1Last) && (0 == ZJDI.InfraredLeft1) && (2 == OpenDoorAvoidHurt.DealingFlag)){
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 6;
                }
			}
            break;
            
		case 5: //���岽�������ȴ��������
			if(++OpenDoorAvoidHurt.DelayCount > 40)
			{
				OpenDoorAvoidHurt.DelayCount = 41;
				if(gSendToMotorDriveStatus.bit.multiPosEND && ZJCOMMONFUJI.bit.SEND)
				{
					OpenDoorAvoidHurt.DelayCount = 0;
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 0;
                    OpenDoorAvoidHurt.DealingFlag = 0;
                    ZhaJiLeftMODE_ID = 0;
                    ZJDI.OpenLeft=1;    //��������
                    ZJLH.LockEnable = GetFuncCodeValue(1,4,29); //�ָ����ʹ��λ
                    
					//OpenDoorAvoidHurt.OpenDoorFJReboundID = 6;							
				}
			}
            break;

        case 6: //���岽�������ȴ��������
			if(++OpenDoorAvoidHurt.DelayCount > 40)
			{
				OpenDoorAvoidHurt.DelayCount = 41;
				if(gSendToMotorDriveStatus.bit.multiPosEND && ZJCOMMONFUJI.bit.SEND)
				{
					OpenDoorAvoidHurt.DelayCount = 0;
                    OpenDoorAvoidHurt.OpenDoorFJReboundID = 0;
                    OpenDoorAvoidHurt.DealingFlag = 0;
                    ZhaJiLeftMODE_ID = 0;
                    ZJDI.OpenRight=1;    //�����ҿ���
                    ZJLH.LockEnable = GetFuncCodeValue(1,4,29); //�ָ����ʹ��λ
                    
					//OpenDoorAvoidHurt.OpenDoorFJReboundID = 6;							
				}
			}
            break;
            
        default:
            break;
            

    }    
}




/*
* ����:���ŷ����ж�
* 
*/
void JudgeOpeningDoorFJ(OPEN_DOOR_FLAG_ENUM dir) //���ŷ����ж�
{
    if(NOT_OPENING_DOOR == dir){    //û���ڿ���
        return;
    }

    if(0 != OpenDoorAvoidHurt.DealingFlag){ //�Ѿ��ڽ��з��д�����
        return;
    }

// ��������ж�    
    if(((1 == ZJDI.InfraredRight1Last) && (1 == ZJDI.InfraredRight1) && (OPENING_LEFT_DOOR == dir)))   //����ʱ��1�������ź�,�ݲ����Ƿ��к���
       // || (((1 == ZhaJi.MODE_ID) || (3 == ZhaJi.MODE_ID) || (4 == ZhaJi.MODE_ID)) && (1 == ZJDI.InfraredFangJia))) //��ˢ��ģʽ�£�����ֻ�з����ź�Ҳ�ᴥ�����ŷ���
    {
        OpenDoorAvoidHurt.OpenDoorFJReboundID = 1;
        OpenDoorAvoidHurt.DealingFlag = 1;
        ZhaJiLeftMODE_ID = 77; //��Ϊ0����default���ֵ�����θ������
        CLOSEDOORMODE_ID = 0;
        return;
    }

    if(((1 == ZJDI.InfraredLeft1Last) && (1 == ZJDI.InfraredLeft1) && (OPENING_RIGHT_DOOR == dir)))   //�ҿ���ʱ��1�������ź�,�ݲ����Ƿ��к���
    {
        OpenDoorAvoidHurt.OpenDoorFJReboundID = 1;
        OpenDoorAvoidHurt.DealingFlag = 2;
        ZhaJiLeftMODE_ID = 77;
        CLOSEDOORMODE_ID = 0;
        return;
    }

// ��������ж�

    /* ����� */

    
}



/*
* ����:���ݺ����ж�����բ���ľ���λ��
* 
*/
void JudgeHumanPos(void)
{
    if((0 == ZJDI.InfraredLeft1) && (0 == ZJDI.InfraredFangJia)){ //���բ��������
        OpenDoorAvoidHurt.LeftHumanPos = 0;
    }
    else if((1 == ZJDI.InfraredLeft1) && (0 == ZJDI.InfraredFangJia)){ //���բ�����˵��Բ���Ų��ᱻ�е�
        OpenDoorAvoidHurt.LeftHumanPos = 1;
    }
    else{
        OpenDoorAvoidHurt.LeftHumanPos = 2;
    }


    if((0 == ZJDI.InfraredRight1) && (0 == ZJDI.InfraredFangJia)){ //�Ҳ�բ��������
        OpenDoorAvoidHurt.RightHumanPos = 0;
    }
    else if((1 == ZJDI.InfraredRight1) && (0 == ZJDI.InfraredFangJia)){ //�Ҳ�բ�����˵��Բ���Ų��ᱻ�е�
        OpenDoorAvoidHurt.RightHumanPos = 1;
    }
    else{
        OpenDoorAvoidHurt.RightHumanPos = 2;
    }
    
}





