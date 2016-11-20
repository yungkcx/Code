#ifndef CODE_H__
# define CODE_H__

#include <stdio.h>
#include <stdbool.h>

bool AND(bool, bool);
bool OR(bool, bool);
bool NOR(bool, bool);
bool NAND(bool, bool);
bool inverter(bool);
bool XOR(bool, bool);
bool HalfAdder(bool, bool, bool*);
bool FullAdder(bool, bool, bool, bool*);
void _8bitAdder(bool[], bool[], bool[], bool, bool*);
void onesComplement(bool, bool[]);
void _8bitAdderOrSubtractor(bool a[], bool b[], bool output[], bool sub, bool *carryOut);

#endif /* CODE_H__ */
