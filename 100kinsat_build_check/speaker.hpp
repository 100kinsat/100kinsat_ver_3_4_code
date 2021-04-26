// speaker.hpp
#pragma once

#include "Arduino.h"

class Speaker {
 public:
  Speaker();
  void beep(int se_type);
  void beep(int se_type, int t_ms);

  static constexpr int WAKEUP = 0;

 private:
  void beep(float *mm, int m_size, int t_ms);
  void noTone(int pin);
  void tone(int pin, int freq, int t_ms);
  static constexpr uint8_t sp = 12;

  static constexpr float mC = 261.626;
  static constexpr float mD = 293.665;
  static constexpr float mE = 329.628;
  static constexpr float mF = 349.228;
  static constexpr float mG = 391.995;
  static constexpr float mA = 440.000;
  static constexpr float mB = 493.883;

  static constexpr int LEDC_CHANNEL_2 = 2;
  static constexpr int LEDC_TIMER_13_BIT = 13;
  static constexpr int LEDC_BASE_FREQ = 5000;

  float wakeup[7] = {mE * 4, mA * 4, mB * 4, mA * 4, mE * 2, mE * 4, mB * 4};

  float doremi[8] = {mC * 2, mD * 2, mE * 2, mF * 2,
                     mG * 2, mA * 2, mB * 2, mC * 4};
};
