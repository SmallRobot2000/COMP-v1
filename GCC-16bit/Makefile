#set -o vi
#ia16-elf-ld -T link.ld startNASM.o main.o -o kernel.bin --oformat binary
#ia16-elf-gcc -march=i8088 -mtune=i8088 -mcmodel=medium -ffreestanding -nostdlib -c main.c -o main.o
#nasm -f elf startNASM.s -o startNASM.o

FIL_LINK = link.ld

CFLAGS= -march=i8088 -mtune=i8088 -mcmodel=tiny -ffreestanding -nostdlib -c
LDFLAGS=-T $(FIL_LINK) --oformat binary
ASFLAGS=-f elf
#useful ndisasm test.bin -b 16
all: prog.bin

prog.bin: startNASM.o asmFunc.o main.o
#ld86 -o $@ $^ $(LDFLAGS)
	ia16-elf-ld -o $@ $^ $(LDFLAGS)
main.o: main.c
#	bcc -o $@ $^ $(CFLAGS)
	ia16-elf-gcc $(CFLAGS) $^ -o $@
startNASM.o: startNASM.s
	nasm -o $@ $^ $(ASFLAGS)

asmFunc.o: asmFunc.s
	nasm -o $@ $^ $(ASFLAGS)

diss:
	ndisasm -b 16 prog.bin

clean:
	rm -f *.o prog.bin