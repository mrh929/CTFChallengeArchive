#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char list[3][0x8];

void init(){
    memset(list, 0, sizeof(list));
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
}

void vul(){
    system("/bin/sh");
}

char* which(int t){
    char *p = malloc(0x10);
    if(t == 1)
        strncpy(p, "st\x00", 3);
    else if(t == 2)
        strncpy(p, "nd\x00", 3);
    else if(t == 3)
        strncpy(p, "rd\x00", 3);
    else
        strncpy(p, "th\x00", 3);
    return p;
}

int main(){
    init();

    puts("It's my great honor that you could come to my flag house!");
    puts("I give you a chance to have three wishes realized.");

    int cnt = 3, t;
    while(cnt){
        puts("which?");
        scanf("%d", &t);
        if((unsigned int)t >= 1){
            printf("OK, tell me your %d%s wish:", t, which(t));
            scanf("%7s", list[t-1]);
            puts("I know it.");
            cnt--;
        }else
            puts("Oh gentleman, this is not allowed.");
        puts("");
    }

    puts("Bye~");
}
