#include "code.h"

bool HalfAdder(bool a, bool b, bool *carryOut)
{
    *carryOut = AND(a, b);
    return XOR(a, b);
}

bool FullAdder(bool a, bool b, bool carryIn, bool *carryOut)
{
    bool tmpSum, tmpCarryOut1, tmpCarryOut2;
    tmpSum = HalfAdder(a, b, &tmpCarryOut1);
    tmpSum = HalfAdder(carryIn, tmpSum, &tmpCarryOut2);
    *carryOut = OR(tmpCarryOut1, tmpCarryOut2);
    return tmpSum;
}

/* Ripple carry. */
void bit8Adder(bool a[], bool b[], bool output[], bool carryIn, bool *carryOut)
{
    for (int i = 8; i > 0; --i) {
        output[i - 1] = FullAdder(a[i - 1], b[i - 1], carryIn, carryOut);
        carryIn = *carryOut;
    }
}

void onesComplement(bool signal, bool a[])
{
    for (int i = 0; i < 8; ++i)
        a[i] = XOR(signal, a[i]);
}

void bit8AdderOrSubtractor(bool a[], bool b[], bool output[], bool sub, bool *carryOut)
{
    /* If a > b, carryOut is 0 and output is right, else carryOut
       is 1 and result is wrong. */
    onesComplement(sub, b);
    bit8Adder(a, b, output, sub, carryOut);
    *carryOut = XOR(*carryOut, sub);
}

void bit8FlipLatchFunc(bit8FlipLatch *fl, bool dataIn[], bool output[], bool w, bool clr)
{
    for (int i = 0; i < 8; ++i)
        output[i] = DFlipLatchFunc(&fl->bits[i], dataIn[i], w, clr);
}

bool selector2_1(bool a, bool b, bool select)
{
    /* If select == 0, then output a, else output b. */
    return OR(AND(b, select), AND(a, !select));
}

void bit8Selector2_1(bool a[], bool b[], bool output[], bool select)
{
    for (int i = 0; i < 8; ++i)
        output[i] = selector2_1(a[i], b[i], select);
}

bool selector8_1(bool d[], bool s[])
{
    /* If s == { 1, 0, 0 }, then select d[0],
       else if s == { 0, 0, 1 }, then select d[4],
       etc. */
    bool tmp[8] = {
        bit4AND(d[0], NOT(s[0]), NOT(s[1]), NOT(s[2])),
        bit4AND(d[1], s[0], NOT(s[1]), NOT(s[2])),
        bit4AND(d[2], NOT(s[0]), s[1], NOT(s[2])),
        bit4AND(d[3], s[0], s[1], NOT(s[2])),
        bit4AND(d[4], NOT(s[0]), NOT(s[1]), s[2]),
        bit4AND(d[5], s[0], NOT(s[1]), s[2]),
        bit4AND(d[6], NOT(s[0]), s[1], s[2]),
        bit4AND(d[7], s[0], s[1], s[2])
    };
    return bit8OR(tmp);
}

void dataDecoder3_8(bool w, bool s[], bool output[])
{
    output[0] = bit4AND(w, NOT(s[0]), NOT(s[1]), NOT(s[2]));
    output[1] = bit4AND(w, s[0], NOT(s[1]), NOT(s[2]));
    output[2] = bit4AND(w, NOT(s[0]), s[1], NOT(s[2]));
    output[3] = bit4AND(w, s[0], s[1], NOT(s[2]));
    output[4] = bit4AND(w, NOT(s[0]), NOT(s[1]), s[2]);
    output[5] = bit4AND(w, s[0], NOT(s[1]), s[2]);
    output[6] = bit4AND(w, NOT(s[0]), s[1], s[2]);
    output[7] = bit4AND(w, s[0], s[1], s[2]);
}
