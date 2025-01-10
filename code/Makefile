PROGRAM = 8088rom

BIN = $(PROGRAM).bin
LST = $(PROGRAM).lst

all: $(BIN)

prog: $(BIN)
$(BIN): $(PROGRAM).asm *.inc
	nasm -o $(BIN) $(PROGRAM).asm
	minipro -p AT28C256 -w $(BIN)
clean:
	rm -f $(BIN) $(LST)

flash: $(BIN)
	minipro -p AT28C256 -w $(BIN)

GAL:
	$(MAKE) -C ../GALing all