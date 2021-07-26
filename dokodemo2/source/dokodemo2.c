#include <stdio.h>
#include <signal.h>
#include <string.h>

short bell;
char master[6];

void handler(){
    puts("The door of flag has closed.");
    raise(SIGKILL);
    //exit(0);
}

void init(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
    alarm(20);
    signal(SIGALRM, handler);
}


void ring(int *p){
    scanf("%llu", p);
}

int main(){
    init();
    puts("You've crossed Dokodemo door.");
    puts("Here is the flag house.");
    puts("Ring the bell if you want to get in.");

    ring(&bell);

    if(!strncmp(master, "AtHome", 6)){
        puts("Ok. Please come in.");
        system("/bin/sh");
    }else{
        puts("Sorry, the master may not be at home.");
    }

    return 0;
}
