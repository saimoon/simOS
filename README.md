## simOS - a tiny x86 kernel

This is a simple x86 kernel I develop mainly to go deep in kernel stuffs.

To test it (in a fast way) use an x86 emulator like qemu (or bochs).

* http://wiki.qemu.org/


#### Build
To compile code:
```
$ make clean
$ make
```

I suggest you to use a GCC cross-compiler (look at http://wiki.osdev.org/GCC_Cross-Compiler). 

#### Run
To test OS using qemu-system-i386:
```
$ sh script/run.sh
```

#### Debug
To debug OS using qemu-system-i386:
```
$ sh scripts/debug.sh
```

then on another shell:
```
$ gdb ./simOS.bin
(gdb) target remote localhost:1234
(gdb) break kernel_main
```

comments to: simone@piemontewireless.net
