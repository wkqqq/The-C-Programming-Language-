#include <stdio.h>

#define LOWER 0     //温度表的下限
#define UPPER 300   //温度表的上限
#define STEP  20    //步长

float FahrToCelsius(float fahr);

void main()
{
    float fahr,celsius;
    fahr = LOWER;
    
    printf("fahr\tcelsius\n");   //打印表头
    
    for(fahr = LOWER; fahr <= UPPER; fahr += STEP){
        celsius = FahrToCelsius(fahr);
        printf("%3.0f\t%6.1f\n",fahr,celsius);
    }
    while(1);
}

float FahrToCelsius(float fahr)
{
    float celsius;
    celsius = (5.0/9.0 * (fahr - 32.0));
    return celsius;
}