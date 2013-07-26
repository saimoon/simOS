CC = i586-elf-gcc
AS = i586-elf-as
CFLAGS = -I./include -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

OBJS = boot.o utils.o console.o mem.o int.o int_vectors.o timer.o kbd.o multiboot.o kernel.o

all: simOS.bin

%.o: %.c
	$(CC) -g -c $*.c -o $*.o $(CFLAGS)

%.o: %.S
	$(CC) -g -c $*.S -o $*.o $(CFLAGS)

%.o: %.s
	$(AS) $*.s -o $*.o

simOS.bin: $(OBJS)
	$(CC) -g -T linker.ld -o simOS.bin -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

clean:
	rm -f *.o *.bin
