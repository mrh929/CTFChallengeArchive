/*
gcc pwn.c -o pwn -lpthread -no-pie
*/
 
#include<stdio.h>
#include<pthread.h>
 
void backdoor(){
    execve("/bin/sh", 0, 0);
}
 
void run(){
    char buf[0x20];
    signed int size;
    puts("how long?");
    read(0, buf, 8);
    size = atoi(buf);

    if(size > 0x20){
        puts("no.");
        return;
    }

    puts("what?");
    write(1, buf, read(0, buf, size));
}

void setbufs(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
}

int main(){
    setbufs();

    pthread_t newthread;
    while(1)
        if(pthread_create(&newthread, 0, run, 0) != -1)
            pthread_join(newthread, 0);
 
}