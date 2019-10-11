#include <stdio.h>

void main()
{
    int c;
    int n_tab,n_space,n_newline;
    n_tab = 0;
    n_space = 0;
    n_newline = 0;
    while((c = getchar()) != EOF){
        if(c == '\t')
            ++n_tab;
            
        if(c == ' ')
            ++n_space;
        
        if(c == '\n')
            ++n_newline;
            
        printf("tab number = %d\n",n_tab);
        printf("space number = %d\n",n_space);
        printf("newline number = %d\n",n_newline);
    }
}