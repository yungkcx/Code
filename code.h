#ifndef CODE_H__
# define CODE_H__

#include "alloca.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define  LOAD   0x10
#define  STORE  0x11
#define  ADD    0x20
#define  SUB    0x21
#define  AWC    0x22  /* Add with Carry */
#define  SWB    0x23  /* Subtract with Borrow */
#define  HALT   0xFF

bool AND(bool, bool);
bool bit4AND(bool a, bool b, bool c, bool d);
bool bit8AND(bool a[]);
bool OR(bool, bool);
bool bit8OR(bool a[]);
bool NOT(bool);
bool NOR(bool, bool);
bool NAND(bool, bool);
bool inverter(bool);
bool XOR(bool, bool);
bool HalfAdder(bool a, bool b, bool *carryOut);
bool FullAdder(bool a, bool b, bool carryIn, bool *carryOut);
void bit8Adder(bool a[], bool b[], bool output[], bool carryIn, bool *carryOut);
void onesComplement(bool signal, bool a[]);
void bit8AdderOrSubtractor(bool a[], bool b[], bool output[], bool sub, bool *carryOut);

typedef bool DFlipLatch;
typedef struct {
    DFlipLatch bits[8];
} bit8FlipLatch;
typedef unsigned short bit16Counter;

bool DFlipLatchFunc(DFlipLatch *df, bool d, bool w, bool clr);
void bit8FlipLatchFunc(bit8FlipLatch *df, bool dataIn[], bool output[], bool w, bool clr);
void bit8Selector2_1(bool in0[], bool in1[], bool output[], bool select);
bool selector8_1(bool d[], bool s[]);
void dataDecoder3_8(bool d, bool s[], bool output[]);
bool bit8RAM(DFlipLatch df[], bool addr[], bool d, bool w);
void bit8RAM8(DFlipLatch df[], bool addr[], bool d[], bool output[], bool w);
void bit8Decoder1_2(bool d[], bool output0[], bool output1[], bool select);
void decoder1_2(bool d, bool *out0, bool *out1, bool select);
void kb64RAM8(DFlipLatch df[], bool addr[], bool d[], bool output[], bool w);
void bulb(bool a[]);
void bit16CounterFunc(bit16Counter *cnt, bool pre, bool d[], bool clr, bool output[]);
int bool8toint(bool a[]);
void computer(int fd);

#define GROUND 0
#define DUMP_BOOL_ARRAY(a, n) \
    do {\
        for (int i = 0; i < n; ++i)\
            printf("%1d", a[i]);\
        putchar('\n');\
    } while (0)

#endif /* CODE_H__ */
