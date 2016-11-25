CC=gcc
PROGRAM=translator
CFLAGS=-Wall -Wno-unused -lm

all: translator.c
	gcc $^ -o ${PROGRAM} ${CFLAGS}

.PHONY: clean
clean:
	rm ${PROGRAM}
