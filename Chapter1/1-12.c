#include <stdio.h>

#define OUT 0
#define IN  1

void main()
{
    int c;
    int statusFlag;
    statusFlag = 0;
    while((c = getchar()) != EOF){
        while((c == ' ') || (c == '\n') || (c == '\t')){
            statusFlag = 1;
            c = getchar();
        }
        if(statusFlag == 1){
            putchar('\n');
            statusFlag = 0;
        }
        putchar(c);
    }
    
}