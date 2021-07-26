#!/bin/sh
qemu-system-x86_64 \
-m 128M \
-kernel ./bzImage \
-initrd  ./rootfs.img \
-append "root=/dev/ram rw console=ttyS0 oops=panic panic=1 quiet kaslr useradd ctf" \
-netdev user,id=t0, -device e1000,netdev=t0,id=nic0 \
-smp cores=2,threads=1 \
-cpu qemu64,+smep \
-monitor /dev/null \
-nographic
