#include <stdio.h>

void main()
{
    int chnum[128];
    int c,i,j;
    
    for(i = 0; i < 128; i++){
        chnum[i] = 0;
    }
    while((c = getchar()) != EOF){
        chnum[c]++;
    }
    
    for(i = 0; i < 128; i++){
        printf("%c\t|",i);
        for(j = 0; j < chnum[i]; j++){
            printf("*");
        }
        printf("\n");
    }
    while(1);
}