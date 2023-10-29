/*
this file needs to bee reviewed
none of the functions are not aplicable
you may need to rewrite them all
or delete all
*/


#include <iostream>
using namespace std;

short dec2binWzerofill(int *Dec);
short dec2binWzerofill2(int *Dec); // personlized function
bool *dec2binWzerofill3(int *Dec);

int main()
{

    // int decimal = 4395553; //0
    // int decimal = 4395555; //1
    int decimal = 4395556; // 2
    // int decimal = 4395557; //3 ??
    int decimal2 = decimal;

    // 010000110001001000100100 => 274722 & 0100

    // cout << dec2binWzerofill2(&decimal);
    bool *cmd = dec2binWzerofill3(&decimal);
    cout << cmd[0] << " " << cmd[1] << " " << decimal << "\n";
    cout << dec2binWzerofill(&decimal2) << " "<< decimal2;

    return 0;
}

short dec2binWzerofill(int *Dec) // ok
{
    short out = 0;

    ((*Dec & 1) > 0) ? out = 0 : 0;
    *Dec = *Dec >> 1;
    ((*Dec & 1) > 0) ? out = 1 : 0;
    *Dec = *Dec >> 1;
    ((*Dec & 1) > 0) ? out = 2 : 0;
    *Dec = *Dec >> 1;
    ((*Dec & 1) > 0) ? out = 3 : 0;
    *Dec = *Dec >> 1;

    return out;
}

short dec2binWzerofill2(int *Dec) // personlized function
{
    bool cmd[4];
    short i = 0;
    short out = 0;

    for (i = 3; i >= 0; i--)
    {
        // cout << i/2;
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

bool *dec2binWzerofill3(int *Dec)
{
    bool cmd[] = {0,0};

    // if ((*Dec & 1) > 0)
    // {
    //     cmd[0] = 0;
    //     cmd[1] = 0;
    // }
    *Dec = *Dec >> 1;
    if ((*Dec & 1) > 0)
    {
        cmd[0] = 1;
        cmd[1] = 0;
    }
    *Dec = *Dec >> 1;
    if ((*Dec & 1) > 0)
    {
        cmd[0] = 0;
        cmd[1] = 1;
    }
    *Dec = *Dec >> 1;
    if ((*Dec & 1) > 0)
    {
        cmd[0] = 1;
        cmd[1] = 1;
    }
    *Dec = *Dec >> 1;

    return cmd;
}
