#include <stdio.h>

#define LOWER 0     //�¶ȱ������
#define UPPER 300   //�¶ȱ������
#define STEP  20    //����

void main()
{
    float fahr,celsius;
    fahr = LOWER;
    
    printf("celsius\tfahr\n");   //��ӡ��ͷ
    
    for(celsius = LOWER; celsius <= UPPER; celsius += STEP){
        fahr = 9.0/5.0 * celsius + 32.0;
        printf("%3.0f\t%6.1f\n",celsius,fahr);
    }
    while(1);
}