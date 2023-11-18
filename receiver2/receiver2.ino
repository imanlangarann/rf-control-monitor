#include <RCSwitch.h>

#define input_signal_debaunce 150
#define indicator 13

bool ledStt[] = { 0, 0, 0, 0 };
bool tempLedStt[] = { 0, 0, 0, 0 };
short leds[] = { 4, 5, 6, 7 };

RCSwitch mySwitch = RCSwitch();

long last_sig = 0;
unsigned long remote_val = 0;
short key = 0;
short i = 0;


void setup() {

  Serial.begin(9600);

  mySwitch.enableReceive(0);
  for (i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], 0);
  }

  pinMode(indicator, OUTPUT);

  Serial.println("setup...");
}

void loop() {

  checkIndicator();

  if (mySwitch.available()) {
    if (mySwitch.getReceivedProtocol() == 2) {
      // if (millis() - last_sig > input_signal_debaunce) {
      //   last_sig = millis();

      remote_val = mySwitch.getReceivedValue();

      dec2binWzerofill(&remote_val);

      Serial.print("Address:");
      Serial.print(remote_val);
      // Serial.print(" - key: ");
      // Serial.println(key);

      if (remote_val == 5) {
        for (i = 0; i < 4; i++) {
          if (ledStt[i] != tempLedStt[i]) {
            ledStt[i] = tempLedStt[i];
            digitalWrite(leds[i], ledStt[i]);
          }
        }
        Serial.print(" - ledStt:");
        for (i = 0; i < 4; i++) {
          Serial.print(ledStt[i]);
          Serial.print(",");
        }
      }
      Serial.println("");




      // }
      // else {
      //   last_sig = millis();
      // }
    }
    mySwitch.resetAvailable();
  }
}


long last_on = 0;
bool indicator_stt = 1;

void checkIndicator() {

  if (millis() - last_on > 3000 && !indicator_stt) {
    last_on = millis();
    indicator_stt = 1;
  }
  if (millis() - last_on > 100 && indicator_stt) {
    last_on = millis();
    indicator_stt = 0;
  }
  digitalWrite(indicator, indicator_stt);
}



void dec2binWzerofill(unsigned long *Dec) {

  for (i = 0; i < 4; i++) {
    if (*Dec & 1) tempLedStt[i] = 1;
    else tempLedStt[i] = 0;
    *Dec = *Dec >> 1;
  }
}
