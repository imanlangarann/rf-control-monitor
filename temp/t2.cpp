#include <iostream>
#include <bitset>
using namespace std;

bool ledStt[] = {1, 0, 1, 1};

char list_to_char(bool *list)
{
    char result = 0;
    for (int i = 0; i < 4; i++)
    {
        if (list[i] == true)
            result |= (1 << i);
    }
    return result;
}

void char_to_list(char ch, bool *lis)
{
    // bool *result = new bool[4];
    int j = 0;
    while (ch != 0)
    {
        if ((ch & 1) == 1)
            lis[j++] = true;
        else
            lis[j++] = false;
        ch >>= 1;
    }
    // return result;
}

int main()
{
    char result = list_to_char(ledStt);

    // bool *ls = char_to_list(result);

    bool *ls = new bool[4];

    char_to_list(result,ls);

    cout << bitset<8>(result) << " - ";
    for(int i = 0 ; i < 4 ; i++){
        cout << ls[i] << ",";
    }

    // cout << ledStt;

    return 0;
}
