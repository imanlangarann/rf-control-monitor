#include <RCSwitch.h>

#define debunce 150
#define indicator 13

enum mode { read,
            learn,
            forget,
            forgetAll } mode = read;

short leds[] = { 4, 5, 6, 7 };
bool ledStt[] = { 0, 0, 0, 0 };
unsigned long rmAddrs[10] = { 21532 };

RCSwitch mySwitch = RCSwitch();
short dec2binWzerofill(unsigned long *Dec);
void checkIndicator();

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

short key = 0;
unsigned long remote_val = 0;

long last_sig = 0;

void loop() {

  checkIndicator();

  if (mode == read) {
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
}


bool is_valid_addr(unsigned long remoteAddress) {
  if (remoteAddress == 0) return false;

  for (unsigned long i : rmAddrs) {
    if (i == remoteAddress)
      return true;
  }
  return false;
}



long last_on = 0;
bool indicator_stt = 0;
bool blink_num = 0;

void checkIndicator() {
  switch (mode) {
    case read:
      if (millis() - last_on > 8000 && !indicator_stt) {
        last_on = millis();
        indicator_stt = 1;
      }
      if (millis() - last_on > 100 && indicator_stt) {
        last_on = millis();
        indicator_stt = 0;
      }
      digitalWrite(indicator, indicator_stt);
      break;

    case learn:
      if (millis() - last_on > 1000 && !indicator_stt) {
        last_on = millis();
        indicator_stt = 1;
      }
      if (millis() - last_on > 100 && indicator_stt) {
        last_on = millis();
        indicator_stt = 0;
      }
      digitalWrite(indicator, indicator_stt);
      break;

    case forget:
      if (!indicator_stt) {
        if (millis() - last_on > 100 && blink_num) {
          last_on = millis();
          blink_num = 0;
          digitalWrite(indicator, 0);
        } else if (millis() - last_on > 1000 && !blink_num) {
          last_on = millis();
          indicator_stt = 1;
          digitalWrite(indicator, 1);
        }
      }
      if (millis() - last_on > 100 && indicator_stt) {
        last_on = millis();
        if (!blink_num) {
          blink_num = 1;
          digitalWrite(indicator, 0);
        } else {
          digitalWrite(indicator, 1);
          indicator_stt = 0;
        }
      }
      break;

    case forgetAll:
      if (millis() - last_on > 100) {
        last_on = millis();
        indicator_stt = !indicator_stt;
      }
      digitalWrite(indicator, indicator_stt);
      break;
  }
}



short dec2binWzerofill(unsigned long *Dec) {

  short out = 0;

  for (i = 0; i < 4; i++) {
    if (*Dec & 1) out = i;
    *Dec = *Dec >> 1;
  }

  return out;
}

/*
new codes for long press from gpt

const int buttonPin = 2;
int buttonState = 0;
unsigned long startTime = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    startTime = millis();
    while (millis() - startTime < 10000) {
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW) {
        Serial.println("Button released before 10 seconds.");
        break;
      }
    }
    if (buttonState == HIGH) {
      Serial.println("Button pressed for 10 seconds.");
    }
  }
}


*/