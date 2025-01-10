#CFLAGS=-0 -c -ansi  #flags for bcc
CFLAGS= -ms -q -zl -S -zu -s
LDFLAGS=-T 0x0100 -0
ASFLAGS=-f obj
#useful ndisasm test.bin -b 16
all: testc.bin

testc.bin: testasm.o testc.o
#ld86 -o $@ $^ $(LDFLAGS)
	wlink option quiet file testasm.o file testc.o  format raw bin name test.bin option NODEFAULTLIBS,verbose,start=initMain ORDER clname CODE segment INIT

testc.o: testc.c
#	bcc -o $@ $^ $(CFLAGS)
	wcc $^ $(CFLAGS)
testasm.o: testasm.asm
#	nasm -o $@ $^ $(ASFLAGS)
	wasm -0 -ms testasm.asm -q


clean:
	rm -f *.o testc.bin