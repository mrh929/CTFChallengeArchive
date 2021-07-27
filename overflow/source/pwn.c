#include<stdio.h>
#include<stdlib.h>

// gcc pwn.c -o pwn -m32 -no-pie -fno-stack-protector

char buf[8] = "/bin/sh";

void banner(){
    puts("                           _____.__                 \n  _______  __ ____________/ ____\\  |   ______  _  __\n /  _ \\  \\/ // __ \\_  __ \\   __\\|  |  /  _ \\ \\/ \\/ /\n(  <_> )   /\\  ___/|  | \\/|  |  |  |_(  <_> )     / \n \\____/ \\_/  \\___  >__|   |__|  |____/\\____/ \\/\\_/  \n                 \\/                                 ");
}

void init(){
    system("clear");
    setbuf(stdin, 0LL);
    setbuf(stdout, 0LL);
    setbuf(stderr, 0LL);
    banner();
}

void backdoor(){
    system("/bin/sh");
}


char *fvck[1010]={0};
void *p;

void setbufs(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
}

int main(){
    // execl("/bin/sh", NULL, NULL);
    // exit(0);
    setbufs();
    char s[8];
    init();
    p = &backdoor;
    printf("Backdoor is at 0x%lx\n", p);

    puts("What do you want to say?");
    read(0, s, 0x100);
}
