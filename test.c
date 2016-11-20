#include "code.h"

static int test_pass = 0;
static int test_count = 0;
static int test_code = 0;

#define TEST_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            test_code = 1;\
        }\
    } while (0)

#define TEST_BOOL_EQ(expect, actual)  TEST_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_logic_gate()
{
    TEST_BOOL_EQ(1, AND(1, 1));
    TEST_BOOL_EQ(0, AND(1, 0));
    TEST_BOOL_EQ(0, AND(0, 1));
    TEST_BOOL_EQ(0, AND(0, 0));
    TEST_BOOL_EQ(1, OR(1, 1));
    TEST_BOOL_EQ(1, OR(1, 0));
    TEST_BOOL_EQ(1, OR(0, 1));
    TEST_BOOL_EQ(0, OR(0, 0));
    TEST_BOOL_EQ(1, NOR(0, 0));
    TEST_BOOL_EQ(0, NOR(1, 1));
    TEST_BOOL_EQ(0, NOR(0, 1));
    TEST_BOOL_EQ(0, NOR(1, 0));
    TEST_BOOL_EQ(1, NAND(1, 0));
    TEST_BOOL_EQ(1, NAND(0, 0));
    TEST_BOOL_EQ(1, NAND(0, 1));
    TEST_BOOL_EQ(0, NAND(1, 1));
    TEST_BOOL_EQ(1, XOR(1, 0));
    TEST_BOOL_EQ(1, XOR(0, 1));
    TEST_BOOL_EQ(0, XOR(1, 1));
    TEST_BOOL_EQ(0, XOR(0, 0));
}

static void test_8bit_adder()
{
    bool a[8] = { 1, 0, 0, 1, 1, 1, 0, 0 };
    bool b[8] = { 1, 1, 0, 1, 0, 1, 0, 1 };
    bool carryIn = 0;
    bool carryOut;
    bool output[8];
    bool expect[8] = { 0, 1, 1, 1, 0, 0, 0, 1 };
    _8bitAdder(a, b, output, carryIn, &carryOut);
    for (int i = 0; i < 8; ++i)
        TEST_BOOL_EQ(expect[i], output[i]);
    TEST_BOOL_EQ(1, carryOut);
}

static void test_8bit_adder_or_subtractor()
{
    bool a[8] = { 1, 1, 1, 1, 1, 1, 0, 1 };
    bool b[8] = { 1, 0, 1, 1, 0, 0, 0, 0 };
    bool expect[8] = { 0, 1, 0, 0, 1, 1, 0, 1 };
    bool carryIn = 0;
    bool carryOut;
    bool output[8];
    _8bitAdderOrSubtractor(a, b, output, 1, &carryOut);
    for (int i = 0; i < 8; ++i)
        TEST_BOOL_EQ(expect[i], output[i]);
    TEST_BOOL_EQ(0, carryOut);
}

static void test_adder()
{
    bool carryOut;
    TEST_BOOL_EQ(0, FullAdder(0, 0, 0, &carryOut));
    TEST_BOOL_EQ(0, carryOut);
    TEST_BOOL_EQ(1, FullAdder(0, 1, 0, &carryOut));
    TEST_BOOL_EQ(0, carryOut);
    TEST_BOOL_EQ(1, FullAdder(1, 0, 0, &carryOut));
    TEST_BOOL_EQ(0, carryOut);
    TEST_BOOL_EQ(0, FullAdder(1, 1, 0, &carryOut));
    TEST_BOOL_EQ(1, carryOut);
    TEST_BOOL_EQ(1, FullAdder(0, 0, 1, &carryOut));
    TEST_BOOL_EQ(0, carryOut);
    TEST_BOOL_EQ(0, FullAdder(0, 1, 1, &carryOut));
    TEST_BOOL_EQ(1, carryOut);
    TEST_BOOL_EQ(0, FullAdder(1, 0, 1, &carryOut));
    TEST_BOOL_EQ(1, carryOut);
    TEST_BOOL_EQ(1, FullAdder(1, 1, 1, &carryOut));
    TEST_BOOL_EQ(1, carryOut);
    test_8bit_adder();
    test_8bit_adder_or_subtractor();
}

static void test()
{
    test_logic_gate();
    test_adder();
}

int main()
{
    test();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return test_code;
}
