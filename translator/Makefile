CC=gcc
PROGRAM=translator
CFLAGS=-Wall -lm

all: translator.c
	gcc $^ -o ${PROGRAM} ${CFLAGS}

.PHONY: clean
clean:
	rm ${PROGRAM}
