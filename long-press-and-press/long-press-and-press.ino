#define btn 8
#define debaunce 150
#define long_press_debaunce 5000

bool btnState = true;
bool lastBtnState = true;
bool reading = true;

unsigned long lastDebounceTime = 0;
unsigned long startLongPress = 0;


void setup() {
  pinMode(btn, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("setup...");
}

void loop() {

  reading = digitalRead(btn);

  if (reading != lastBtnState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debaunce) {

    if (reading != btnState) {
      btnState = reading;
      // once its reed this will run
      // Serial.print("state:");

      if (!btnState) {
        Serial.println("press");
        startLongPress = millis();
      } else {
        Serial.println("release");
      }
    }
  }

  if (!btnState) {
    // Serial.print("press");
    if (millis() - startLongPress > long_press_debaunce) {  // long press
      Serial.println("long");
      while (!digitalRead(btn))
        ;
      btnState = true;
      Serial.println("release");
      // Serial.print(millis() - startLongPress);
    }
  }

  lastBtnState = reading;



  // if(digitalRead(btn) != btnState){
  //   if(millis() - start_press > debaunce){
  //     btnState = digitalRead(btn);
  //   }
  //   start_press = millis();
  // }
}
