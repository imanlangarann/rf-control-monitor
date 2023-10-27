#include <RCSwitch.h>

// int leds[] = {4,5,6,7};

RCSwitch mySwitch = RCSwitch();
short dec2binWzerofill(unsigned long *Dec);

void setup()
{
    Serial.begin(9600);
    mySwitch.enableReceive(0);
}

short key = 0;
unsigned long remote_val = 0;

void loop()
{
    if (mySwitch.available())
    {

        remote_val = mySwitch.getReceivedValue();
        key = dec2binWzerofill(&remote_val);

        Serial.print("Address: ");
        Serial.print(remote_val);
        Serial.print(" - key: ");
        Serial.println(key);

        mySwitch.resetAvailable();
    }
}

short dec2binWzerofill(unsigned long *Dec)
{
    bool cmd[4];
    short i = 0;

    short out = 0;

    for (i = 3; i >= 0; i--)
    {
        cmd[i] = ((*Dec & 1) > 0) ? 1 : 0;
        // out <<= 1;
        // if((*Dec & 1) > 0) out |= 1;
        *Dec = *Dec >> 1;
    }

    for (bool t : cmd)
    {
        out <<= 1;
        out |= t;
    }

    // cmd[4] = '\0';

    return out;
}