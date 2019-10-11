#include <stdio.h>

#define LOWER 0     //温度表的下限
#define UPPER 300   //温度表的上限
#define STEP  20    //步长

void main()
{
    float fahr,celsius;
    fahr = LOWER;
    
    printf("fahr\tcelsius\n");   //打印表头
    
    for(fahr = UPPER; fahr >= LOWER; fahr -= STEP){
        celsius = (5.0/9.0 * (fahr - 32.0));
        printf("%3.0f\t%6.1f\n",fahr,celsius);
    }
    while(1);
}