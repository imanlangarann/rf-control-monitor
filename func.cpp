#include <iostream>
using namespace std;

short dec2binWzerofill(int *Dec);

int main()
{
    int decimal = 4395556;
    // 010000110001001000100100 => 274722 & 0100

    cout << dec2binWzerofill(&decimal);

    return 0;
}

short dec2binWzerofill(int *Dec) // personlized function
{
    bool cmd[4];
    short i = 0;
    short out = 0;

    for (i = 3; i >= 0; i--)
    {
        cmd[i] = ((*Dec & 1) > 0) ? 1 : 0;
        *Dec = *Dec >> 1;
    }

    for (bool t : cmd)
    {
        out <<= 1;
        out |= t;
    }

    return out;
}

static char *dec2binWzerofill(unsigned long Dec, unsigned int bitLength) // function in the document
{
    static char bin[64];
    unsigned int i = 0;

    while (Dec > 0)
    {
        bin[32 + i++] = ((Dec & 1) > 0) ? '1' : '0';
        Dec = Dec >> 1;
    }

    for (unsigned int j = 0; j < bitLength; j++)
    {
        if (j >= bitLength - i)
        {
            bin[j] = bin[31 + i - (j - (bitLength - i))];
        }
        else
        {
            bin[j] = '0';
        }
    }
    bin[bitLength] = '\0';

    return bin;
}