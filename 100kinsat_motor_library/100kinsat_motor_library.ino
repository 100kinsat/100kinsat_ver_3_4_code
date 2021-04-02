#include "motor.hpp"

Motor motor = Motor();

void setup() {
}

void loop() {
  motor.ccw(80);
  delay(1000);

  motor.stop();
  delay(1000);

  motor.cw(80);
  delay(1000);

  motor.stop();
  delay(1000);
}
