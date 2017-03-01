CC=gcc
CFLAGS=-Wall
PROGRAM=calculator
TEST=test

all: main.c calculator.c
	${CC} $^ -o ${PROGRAM} ${CFLAGS}

test: test.c calculator.c
	${CC} $^ -o ${TEST} ${CFLAGS}

.PHONY: clean
clean:
	rm -f ${PROGRAM} ${TEST}
	cd translator && make clean
