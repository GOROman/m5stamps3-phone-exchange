#include <Arduino.h>

#include "exchange.h"

Exchange exchange;

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("M5StampS3 Phone Exchange boot");
  exchange.begin();
}

void loop() {
  exchange.update(millis());
  delay(5);
}

