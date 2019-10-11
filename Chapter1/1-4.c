#include <stdio.h>

#define LOWER 0     //温度表的下限
#define UPPER 300   //温度表的上限
#define STEP  20    //步长

void main()
{
    float fahr,celsius;
    fahr = LOWER;
    
    printf("celsius\tfahr\n");   //打印表头
    
    for(celsius = LOWER; celsius <= UPPER; celsius += STEP){
        fahr = 9.0/5.0 * celsius + 32.0;
        printf("%3.0f\t%6.1f\n",celsius,fahr);
    }
    while(1);
}