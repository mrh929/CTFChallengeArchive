#include<stdio.h>
#include<string.h>

// gcc dokodemo1.c -o dokodemo1 -z execstack -masm=intel

void init(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
}

int main(){
    char s[0x1000];
    init();
    puts("This is a Dokodemo door.");
    puts("Where are you going?");
    gets(s);

    asm("jmp rsp;");
    return 0;
}
