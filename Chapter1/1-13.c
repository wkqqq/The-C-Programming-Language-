/*
    单词字母计数并打印水平直方图
*/
#include <stdio.h>

#define SHUIPING 0  //1打印水平直方图 0 打印垂直直方图

void main()
{
    int c,i,j,num,wstatus;
    int nw[10];
    int maxnum; //垂直直方图时最大值容器
    
    num = maxnum = 0;
    wstatus = 1;
    for(i = 0; i < 10; i++){
        nw[i] = 0;
    }
    
    while((c = getchar()) != EOF){
        if((c == ' ') || (c == '\t') || (c == '\n')){
            wstatus = 0;
        }
        else if(wstatus == 0){  //单词第一个字母开始时
            wstatus = 1;
            nw[num]++;
            num = 0;
        }
        if(wstatus == 1){
            num++;
        }      
    }
    if(wstatus == 0){
        nw[num]++;
    }
#if SHUIPING
    for(i = 1; i < 10; i++){
        printf("%d\t|",i);
        for(j = 0; j < nw[i]; j++){
            printf("*");
        }
        if(nw[i] > 0)
            printf("(%d)\n\t|\n",nw[i]);
        else
            printf("\n\t|\n");
    }
#else
    for(i = 1; i < 10; i++){
        if(nw[i] > maxnum)
            maxnum = nw[i];
    }
    //开始打印
    for(i = 1; i < 10; i++){
        printf("(%d)     ",nw[i]);
    }
    printf("\n");
    for(i = maxnum; i > 0; i--){
        for(j = 1; j < 10; j++){
            if(nw[j] == maxnum){
                printf("****");
                nw[j]--;
            }
            else
                printf("    ");
            
            printf("    ");
        }
        printf("\n");
        maxnum--;
    }
    for(i = 1; i < 10; i++){
        printf(" %d      ",i);
    }
#endif
    while(1);
}