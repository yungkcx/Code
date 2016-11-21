#ifndef CODE_H__
# define CODE_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

bool AND(bool, bool);
bool OR(bool, bool);
bool NOR(bool, bool);
bool NAND(bool, bool);
bool inverter(bool);
bool XOR(bool, bool);
bool HalfAdder(bool, bool, bool*);
bool FullAdder(bool, bool, bool, bool*);
void bit8Adder(bool[], bool[], bool[], bool, bool*);
void onesComplement(bool, bool[]);
void bit8AdderOrSubtractor(bool[], bool[], bool[], bool, bool*);

typedef bool DFlipLatch;
typedef struct {
    DFlipLatch bits[8];
} bit8FlipLatch;

bool DFlipLatchFunc(DFlipLatch*, bool, bool);
void bit8FlipLatchFunc(bit8FlipLatch *fl, bool[], bool[], bool);
void bit8Selector(bool[], bool[], bool[], bool);

#endif /* CODE_H__ */
