#include <EEPROM.h>


#define number_of_addresses 10

unsigned long rmAddrs[number_of_addresses] = {};

void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  EEPROM.get(0, rmAddrs);

  for (short i = 0; i < number_of_addresses; i++) {
    Serial.print(rmAddrs[i]);
    Serial.print(",");
  }
  Serial.println("\ndone");

  Serial.println("to write zeros to rmAddrs enter 1");
}

void loop() {
  if (Serial.available()) {
    if (Serial.parseInt() == 1) {
      for (short i = 0; i < number_of_addresses; i++) {
        rmAddrs[i] = 0;
        Serial.print(rmAddrs[i]);
        Serial.print(",");
      }
      EEPROM.put(0, rmAddrs);
    }
  }
}
