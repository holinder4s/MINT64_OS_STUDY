make
qemu-system-x86_64 -s -S -no-kvm -L . -m 64 -fda ./Disk.img -rtc base=localtime -M pc &
gdb -ix ./Disk.img\
    -ex 'target remote localhost:1234'\
    -ex 'set architecture i8086'\
    -ex 'break *0x7c00'\
    -ex 'continue'