#include "code.h"

bool bit8AND(bool a[])
{
    return AND(
            AND(AND(a[0], a[1]), AND(a[2], a[3])),
            AND(AND(a[4], a[5]), AND(a[6], a[7]))
            );
}

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

void bit8FlipLatchFunc(bit8FlipLatch *fl, bool dataIn[], bool output[], bool write)
{
    for (int i = 0; i < 8; ++i)
        output[i] = DFlipLatchFunc(&fl->bits[i], dataIn[i], write);
}

bool selector2_1(bool a, bool b, bool select)
{
    /* If select == 0, then output a, else output b. */
    return OR(AND(b, select), AND(a, !select));
}

void bit8Selector(bool a[], bool b[], bool output[], bool select)
{
    for (int i = 0; i < 8; ++i)
        output[i] = selector2_1(a[i], b[i], select);
}
