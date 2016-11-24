CC=gcc
CFLAGS=-Wall
PROGRAM=computer
TEST=test

all: main.c computer.c
	${CC} $^ -o ${PROGRAM} ${CFLAGS}

test: test.c computer.c
	${CC} $^ -o ${TEST} ${CFLAGS}

.PHONY: clean
clean:
	rm ${PROGRAM} ${TEST}
