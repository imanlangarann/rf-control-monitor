#include <RCSwitch.h>
#include <EEPROM.h>
#include <avr/wdt.h>

#define indicator 13
#define btn 8

#define btn_debaunce 50
#define input_signal_debaunce 150
#define long_press_debaunce 5000
#define number_of_addresses 10

enum mode { read,
            learn,
            forget,
            forgetAll } mode = read;

short leds[] = { 4, 5, 6, 7 };
bool ledStt[] = { 0, 0, 0, 0 };
unsigned long rmAddrs[number_of_addresses] = {};

bool btnState = true;
bool lastBtnState = true;
bool reading = true;

unsigned long lastDebounceTime = 0;
unsigned long startLongPress = 0;



short key = 0;
unsigned long remote_val = 0;

long last_sig = 0;

bool isPressed = false, isLongPressed = false;
bool should_send = false;

long last_on = 0;
bool indicator_stt = 1;
bool blink_num = 0;


RCSwitch mySwitch = RCSwitch();
short dec2binWzerofill(unsigned long *Dec);
void checkIndicator();
void checkBtn(bool *isPressed, bool *isLongPressed);
void goto_mode(byte m);


short i = 0;

void setup() {

  Serial.begin(9600);
  mySwitch.enableReceive(0);
  mySwitch.enableTransmit(9);
  mySwitch.setProtocol(2);
  mySwitch.setRepeatTransmit(4);

  for (i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], 0);
  }

  /////////read remote addresses from eeprom///////////
  EEPROM.get(0, rmAddrs);
  /////////////////////////////////////////////////////

  pinMode(indicator, OUTPUT);
  digitalWrite(indicator, indicator_stt);
  pinMode(btn, INPUT_PULLUP);

  Serial.println("setup...");

  // Serial.println(list_to_char(ledStt),DEC);
  // mySwitch.send("000000000001010100010001");
  send_data();

  wdt_enable(WDTO_500MS);
}


char tmp = 0;
void send_data() {
  tmp = list_to_char(ledStt);
  Serial.println(tmp, BIN);
  mySwitch.send(tmp, 8);
  // mySwitch.send(0b10000,5);
}




void loop() {

  checkIndicator();

  ///////////////////
  // isPressed = false;
  // isLongPressed = false;
  // checkBtn(&isPressed, &isLongPressed);

  // if (isPressed) { Serial.println("pressed"); }
  // if (isLongPressed) {
  //   Serial.println("long pressed");
  //   gotowhile();
  // }
  ///////////////////


  if (mode == read) {

    isPressed = false;
    checkBtn(&isPressed, &isLongPressed);
    if (isPressed) {
      startLongPress = millis();
      digitalWrite(indicator, 1);
      while (millis() - startLongPress < 3000) {

        wdt_reset();

        isPressed = false;
        checkBtn(&isPressed, &isLongPressed);
        if (isPressed) {
          goto_mode(forget);
          startLongPress = millis();
          break;
        }
      }
      if (mode == read) {
        goto_mode(learn);
        startLongPress = millis();
      }
      digitalWrite(indicator, 0);
    }

    if (mySwitch.available()) {
      if (millis() - last_sig > input_signal_debaunce) {
        last_sig = millis();

        remote_val = mySwitch.getReceivedValue();
        key = dec2binWzerofill(&remote_val);

        if (is_valid_addr(remote_val)) {
          ledStt[key] = !ledStt[key];
          digitalWrite(leds[key], ledStt[key]);

          // last_on = millis();
          // indicator_stt = 1;
          should_send = true;
          // delay(input_signal_debaunce);
          // send_data();
        }

        Serial.print("Address: ");
        Serial.print(remote_val);
        Serial.print(" - key: ");
        Serial.println(key);

      } else {
        last_sig = millis();
      }

      mySwitch.resetAvailable();
      if (should_send) {
        should_send = false;

        delay(input_signal_debaunce);
        wdt_reset();
        delay(input_signal_debaunce);
        wdt_reset();
        /// add wdt of reset
        send_data();
      }
    }
  }

  else if (mode == forget) {

    checkIncativity();

    isPressed = false;
    checkBtn(&isPressed, &isLongPressed);
    if (isPressed) {
      // goto_mode(forgetAll);
      while (!digitalRead(btn)) {
        wdt_reset();

        isLongPressed = false;
        checkBtn(&isPressed, &isLongPressed);
        if (isLongPressed) {
          Serial.println("forget all");  //forget all function
          remove_all_address();
          digitalWrite(indicator, 1);
          while (!digitalRead(btn))
            wdt_reset();
          digitalWrite(indicator, 0);
          goto_mode(read);
          break;
        }

        if (millis() - last_on > 100) {
          last_on = millis();

          indicator_stt = !indicator_stt;
        }
        digitalWrite(indicator, indicator_stt);
        delay(btn_debaunce);
      }


      if (mode == forget) {
        goto_mode(read);
      }
    }

    if (mySwitch.available()) {
      if (millis() - last_sig > input_signal_debaunce) {
        last_sig = millis();

        remote_val = mySwitch.getReceivedValue();
        key = dec2binWzerofill(&remote_val);

        // rmAddrs.remove(remote_val);  ////////////////////////////////////////////////
        remove_address(remote_val);
        Serial.println("remote removed.");
        goto_mode(read);

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

  else if (mode == learn) {
    checkIncativity();

    isPressed = false;
    checkBtn(&isPressed, &isLongPressed);
    if (isPressed) {
      while (!digitalRead(btn)) {

        wdt_reset();

        if (millis() - last_on > 100) {
          last_on = millis();

          indicator_stt = !indicator_stt;
        }
        digitalWrite(indicator, indicator_stt);
        delay(btn_debaunce);
      }

      goto_mode(read);
    }

    if (mySwitch.available()) {
      if (millis() - last_sig > input_signal_debaunce) {
        last_sig = millis();

        remote_val = mySwitch.getReceivedValue();
        key = dec2binWzerofill(&remote_val);

        // rmAddrs.add(remote_val);  ////////////////////////////////////////////////
        add_address(remote_val);
        Serial.println("remote added.");
        goto_mode(read);

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


  wdt_reset();
}


char list_to_char(bool *list) {
  char result = 80;  // => address:5
  for (i = 0; i < 4; i++) {
    if (list[i] == true)
      result |= (1 << i);
  }
  return result;
}


void checkIncativity() {
  if (mode == learn || mode == forget) {
    // Serial.println((millis() - startLongPress));
    if (millis() - startLongPress > 30000) {
      goto_mode(read);
    }
  }
}

// void gotowhile() {
//   while (!digitalRead(btn))
//     ;
// }


void goto_mode(byte m) {
  mode = m;
  Serial.println(mode);
}


void checkBtn(bool *isPressed, bool *isLongPressed) {
  reading = digitalRead(btn);
  if (reading != lastBtnState)
    lastDebounceTime = millis();


  if ((millis() - lastDebounceTime) > btn_debaunce) {

    if (reading != btnState) {
      btnState = reading;
      // once its reed this will run

      if (!btnState) {
        // Serial.println("press");
        *isPressed = true;
        startLongPress = millis();
        // go_next_mode();
      }
      // else {
      //   Serial.println("release");
      // }
    }
  }

  if (!btnState) {
    if (millis() - startLongPress > long_press_debaunce) {  // long press
      // Serial.println("long");
      *isLongPressed = true;

      // while (!digitalRead(btn));

      btnState = true;
      Serial.println("release");
    }
  }

  lastBtnState = reading;
}


bool is_valid_addr(unsigned long remoteAddress) {
  if (remoteAddress == 0) return false;

  for (i = 0; i < number_of_addresses; i++) {
    if (rmAddrs[i] == remoteAddress)
      return true;
  }
  return false;
}

void add_address(unsigned long new_address) {
  if (!is_valid_addr(new_address)) {
    for (i = 0; i < number_of_addresses; i++) {
      if (rmAddrs[i] == 0) {
        rmAddrs[i] = new_address;
        // EEPROM.put(0, rmAddrs);
        break;
      }
    }
  }
}

void remove_address(unsigned long remote_address) {
  for (i = 0; i < number_of_addresses; i++) {
    if (rmAddrs[i] == remote_address) {
      rmAddrs[i] = 0;
      // EEPROM.put(0, rmAddrs);
      break;
    }
  }
}

void remove_all_address() {
  for (byte i = 0; i < number_of_addresses; i++) {
    rmAddrs[i] = 0;
  }
  // EEPROM.put(0, rmAddrs);
}




void checkIndicator() {
  switch (mode) {
    case read:
      if (millis() - last_on > 3000 && !indicator_stt) {
        last_on = millis();
        indicator_stt = 1;
        send_data();
      }
      if (millis() - last_on > 100 && indicator_stt) {
        last_on = millis();
        indicator_stt = 0;
      }
      digitalWrite(indicator, indicator_stt);
      break;

    case learn:
      if (millis() - last_on > 800 && !indicator_stt) {
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
