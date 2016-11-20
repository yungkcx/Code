CC=gcc

all:
	${CC} *.c -o computer

.PHONY: clean
clean:
	rm computer
