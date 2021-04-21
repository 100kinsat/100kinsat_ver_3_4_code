#include "cansat_sd.hpp"

CanSatSd sd = CanSatSd();

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  sd.writeFile(SD, "/cansat.txt", "Hello ");
  sd.appendFile(SD, "/cansat.txt", "CanSat!\n");

  Serial.println("done.");
}

void loop() {}
