#include <RCSwitch.h>

#define debunce 150

short leds[] = { 4, 5, 6, 7 };
bool ledStt[] = { 0, 0, 0, 0 };
unsigned long rmAddrs[10] = { 21532 };

RCSwitch mySwitch = RCSwitch();
short dec2binWzerofill(unsigned long *Dec);

short i = 0;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);

  for (i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], 0);
  }

  Serial.println("setup...");
}

short key = 0;
unsigned long remote_val = 0;

long last_sig = 0;

void loop() {
  if (mySwitch.available()) {

    if (millis() - last_sig > debunce) {
      last_sig = millis();

      remote_val = mySwitch.getReceivedValue();
      key = dec2binWzerofill(&remote_val);

      if (is_valid_addr(remote_val)) {
        ledStt[key] = !ledStt[key];
        digitalWrite(leds[key], ledStt[key]);
      }

      Serial.print("Address: ");
      Serial.print(remote_val);
      Serial.print(" - key: ");
      Serial.println(key);

    } else {
      last_sig = millis();
    }


    mySwitch.resetAvailable();
  }
}


bool is_valid_addr(unsigned long remoteAddress) {
  if(remoteAddress == 0) return false;

  for (unsigned long i : rmAddrs) {
    if (i == remoteAddress)
      return true;
  }
  return false;
}


short dec2binWzerofill(unsigned long *Dec) {

  short out = 0;

  for (i = 0; i < 4; i++) {
    if (*Dec & 1) out = i;
    *Dec = *Dec >> 1;
  }

  return out;
}
