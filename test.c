//“—º”√‹
#include <stdio.h>

#define Uint16 unsigned short int
#define Uint32 unsigned long
#define MAXUINT32   0xFFFFFFFF
void test(void);
void main(void)
{
    Uint32 m_data;
    m_data = 777;
    test();
    printf("test = %d \n",m_data);
    while(1);
}

void test()
{
    m_data = 100;
}