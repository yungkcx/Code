#include "code.h"

bool bit8RAM(DFlipLatch df[], bool addr[], bool d, bool w)
{
    bool data[8] = {0};
    dataDecoder3_8(w, addr, data);
    for (int i = 0; i < 8; ++i)
        data[i] = DFlipLatchFunc(&df[i], d, data[i], 0);
    return selector8_1(data, addr);
}
