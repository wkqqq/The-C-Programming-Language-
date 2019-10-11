#include <stdio.h>

#define OUT 0
#define IN  1

void main()
{
    int c,nl,nw,nc,status;
    status = OUT;
    nl = nw = nc = 0;
    while((c = getchar()) != EOF){
        ++nc;
        if(c == '\n')
            ++nl;
        if((c == ' ') || (c == '\n') || (c == '\t')){
            status = OUT;
        }
        else if(status == OUT){
            status = IN;
            ++nw;
        }
        
        printf("%d %d %d\n",nl,nw,nc);
    }
    
}