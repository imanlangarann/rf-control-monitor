#include <RCSwitch.h>

short leds[] = {4,5,6,7};
unsigned long rmAddrs = []; // list of remote addresses

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

	//outIndex = leds[key];

        Serial.print("Address: ");
        Serial.print(remote_val);
        Serial.print(" - key: ");
        Serial.println(key);

	if(is_valid_addr(remote_val)){
		//do something
	}

        mySwitch.resetAvailable();
    }
}

bool is_valid_addr(unsigned long remoteAddress){
	for(unsigned long i : rmAddrs){
		if (i==remoteAddress)
			return true;
	}
	return false;
}

/*
short getOutIndex(short key){
	// key is 1 2 4 8

	//log key in base 2

	//return leds[logkey];
}*/

short dec2binWzerofill(unsigned long *Dec)
{
   // bool cmd[4];
   // short i = 0;

    short out = 0;
///////
    out <<= 1;
    ((*Dec & 1) > 0) ? out |= 1;
    *Dec = *Dec >> 1;
    ((*Dec & 1) > 0) ? out |= 1;
    *Dec = *Dec >> 1;
    out <<= 1;
    ((*Dec & 1) > 0) ? out |= 1;
    *Dec = *Dec >> 1;
    ((*Dec & 1) > 0) ? out |= 1;
    *Dec = *Dec >> 1;
///////
/*
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
    }*/

    // cmd[4] = '\0';

    return out;
}
