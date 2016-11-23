CC=gcc
CFLAGS=-Wall
PROGRAM=computer
TEST=test

all:
	${CC} main.c computer.c -o ${PROGRAM} ${CFLAGS}

test:
	${CC} test.c computer.c -o ${TEST} ${CFLAGS}

.PHONY: clean
clean:
	rm ${PROGRAM} ${TEST}
