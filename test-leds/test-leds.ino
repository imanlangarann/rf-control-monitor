short leds[] = { 4, 5, 6, 7 };

short i = 0;

void setup() {
  
  for (i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], 0);
  }

}

void loop() {

  for (i = 0; i < 4; i++) {
    digitalWrite(leds[i], 1);
    delay(100);
  }
  
  for (i = 0; i < 4; i++) {
    digitalWrite(leds[i], 0);
    delay(100);
  }

}
