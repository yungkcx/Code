#include "code.h"

bool AND(bool a, bool b)
{
    return a & b;
}

bool bit4AND(bool a, bool b, bool c, bool d)
{
    return AND(AND(a, b), AND(c, d));
}

bool bit8AND(bool a[])
{
    return AND(
            AND(AND(a[0], a[1]), AND(a[2], a[3])),
            AND(AND(a[4], a[5]), AND(a[6], a[7]))
            );
}

bool OR(bool a, bool b)
{
    return a | b;
}

bool bit8OR(bool a[])
{
    return OR(
            OR(OR(a[0], a[1]), OR(a[2], a[3])),
            OR(OR(a[4], a[5]), OR(a[6], a[7]))
            );
}

bool NOT(bool a)
{
    return !a;
}

bool NOR(bool a, bool b)
{
    return NOT(OR(a, b));
}

bool NAND(bool a, bool b)
{
    return NOT(AND(a, b));
}

bool XOR(bool a, bool b)
{
    return a ^ b;
}

bool DFlipLatchFunc(DFlipLatch *fl, bool d, bool clk, bool clr)
{
    if (clr)
        *fl = 0;
    else if (clk)
        *fl = d;
    else
        ; /* Clk is 0, do nothing. */
    return *fl;
}
