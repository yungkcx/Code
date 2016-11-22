#ifndef CODE_H__
# define CODE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

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

bool DFlipLatchFunc(DFlipLatch *fl, bool d, bool w, bool clr);
void bit8FlipLatchFunc(bit8FlipLatch *fl, bool dataIn[], bool output[], bool w, bool clr);
void bit8Selector2_1(bool a[], bool b[], bool output[], bool select);
bool selector8_1(bool d[], bool s[]);
void dataDecoder3_8(bool d, bool s[], bool output[]);
bool bit8RAM(DFlipLatch df[], bool addr[], bool d, bool w);

#endif /* CODE_H__ */
