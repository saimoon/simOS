qemu-system-i386 -s -S -kernel simOS.bin

# then use:
# gdb ./simOS.bin
# target remote localhost:1234
# break kernel_main
