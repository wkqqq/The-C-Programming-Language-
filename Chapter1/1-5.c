#include <stdio.h>

#define LOWER 0     //�¶ȱ������
#define UPPER 300   //�¶ȱ������
#define STEP  20    //����

void main()
{
    float fahr,celsius;
    fahr = LOWER;
    
    printf("fahr\tcelsius\n");   //��ӡ��ͷ
    
    for(fahr = UPPER; fahr >= LOWER; fahr -= STEP){
        celsius = (5.0/9.0 * (fahr - 32.0));
        printf("%3.0f\t%6.1f\n",fahr,celsius);
    }
    while(1);
}