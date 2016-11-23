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

bool DFlipLatchFunc(DFlipLatch *df, bool d, bool clk, bool clr)
{
    if (clr)
        *df = 0;
    else if (clk)
        *df = d;
    else
        ; /* Clk is 0, do nothing. */
    return *df;
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

void bit8FlipLatchFunc(bit8FlipLatch *df, bool dataIn[], bool output[], bool w, bool clr)
{
    for (int i = 0; i < 8; ++i)
        output[i] = DFlipLatchFunc(&df->bits[i], dataIn[i], w, clr);
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

/* 8*1 RAM. */
bool bit8RAM(DFlipLatch df[], bool addr[], bool d, bool w)
{
    bool data[8] = {0};
    dataDecoder3_8(w, addr, data);
    for (int i = 0; i < 8; ++i)
        data[i] = DFlipLatchFunc(&df[i], d, data[i], 0);
    return selector8_1(data, addr);
}

/* 8*8 RAM, sizeof(addr) is 3, sizeof(df) is 8*8, sizeof(d) is 8. */
void bit8RAM8(DFlipLatch df[], bool addr[], bool d[], bool output[], bool w)
{
    for (int i = 0; i < 8*8; i += 8)
        output[i/8] = bit8RAM(df + i, addr, d[i], w);
}

void decoder1_2(bool d, bool *out0, bool *out1, bool select)
{
    *out0 = AND(d, NOT(select));
    *out1 = AND(d, select);
}

void bit8Decoder1_2(bool d[], bool output0[], bool output1[], bool select)
{
    for (int i = 0; i < 8; ++i)
        decoder1_2(d[i], &output0[i], &output1[i], select);
}

/* 64k*8 RAM, sizeof(addr) is 16, sizeof(df) is 64k*8, sizeof(d) is 8. */
void kb64RAM8(DFlipLatch df[], bool addr[], bool d[], bool output[], bool w)
{
    unsigned short offset = 0;
    for (int i = 15; i >= 0; --i) {
        offset <<= 1;
        offset |= addr[i];
    }
    for (int i = 0; i < 8; ++i)
        output[i] = DFlipLatchFunc(&df[offset*8 + i], d[i], w, 0);
}

void bulb(bool a[])
{
    for (int i = 0; i < 8; ++i)
        printf("%s", a[i] ? "○ " : "● ");
    putchar('\n');
}

void bit16CounterFunc(bit16Counter *cnt, bool pre, bool clr, bool output[])
{
    unsigned short tmp;
    if (pre)
        tmp = *cnt = 0xffff;
    else if (clr)
        tmp = *cnt = 0;
    else
        tmp = (*cnt)++;
    for (int i = 0; i < 16; ++i) {
        output[i] = tmp & 0x01;
        tmp >>= 1;
    }
}

void computer()
{
    bool clk, clr = 0;
    DFlipLatch *ram = alloca(64*1024*8);
    bit8FlipLatch fl;
    bool addr[16];
    bool ram_out[8], df_out[8], adder_out[8];
    bool data[8];
    bool ram_write = 0;
    bool ground = 0;
    bit16Counter cnt;
    bool carryOut;
    bool pre = 0;

    setbuf(stdin, NULL);
    clk = 1;
    while (1) {
        puts("/* clr */");
        scanf("%1d", (int*) &clr);
        bit16CounterFunc(&cnt, pre, clr, addr);
        puts("/* RAM write: */");
        scanf("%1d", (int*) &ram_write);
        if (ram_write) {
            int tmp;
            puts("/* RAM Control Panel */");
            for (int i = 0; i < 16; ++i) {
                scanf("%1d", &tmp);
                addr[16-i-1] = tmp;
            }
            for (int i = 0; i < 8; ++i) {
                scanf("%1d", &tmp);
                data[i] = tmp;
            }
        }
        kb64RAM8(ram, addr, data, ram_out, ram_write);
        bit8Adder(ram_out, df_out, adder_out, ground, &carryOut);
        bit8FlipLatchFunc(&fl, adder_out, df_out, clk, clr);
        bulb(df_out);
        //DUMP_BOOL_ARRAY(df_out, 8);
    }
}
