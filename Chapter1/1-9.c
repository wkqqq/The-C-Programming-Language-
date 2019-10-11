#include <stdio.h>
#include <conio.h>

void main()
{
    int c;
    int spaceFlag;
    spaceFlag = 0;
    while((c = getchar()) != EOF){
        while(c == ' '){
            spaceFlag = 1;
            c = getchar();
        }
        if(spaceFlag == 1){
            putchar(' ');
            spaceFlag = 0;
        }
        
        putchar(c);
    }
}