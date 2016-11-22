CC=gcc
CFLAGS=-Wall
PROGRAM=computer

all:
	${CC} *.c -o ${PROGRAM} ${CFLAGS}

.PHONY: clean
clean:
	rm ${PROGRAM}
