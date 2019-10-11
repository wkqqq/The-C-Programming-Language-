#include <stdio.h>
#include <conio.h>

void main()
{
    int c;
    while((c = _getch()) != EOF){
        if(c == '\t')
            printf("\\t");    
        else if(c == '\b')
            printf("\\b");
        else if(c == '\\')
            printf("\\\\");
        else
            putchar(c);
    }
}