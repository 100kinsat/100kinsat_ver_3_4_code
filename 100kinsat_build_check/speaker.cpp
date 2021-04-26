#include "speaker.hpp"

#include "Arduino.h"

Speaker::Speaker() {
  pinMode(sp, OUTPUT);
}

/**
 * @brief 音を鳴らす
 *
 * @param se_type
 */
void Speaker::beep(int se_type) {
  beep(se_type, 200);
}

/**
 * @brief 秒数を指定して音を鳴らす
 *
 * @param se_type
 * @param t_ms
 */
void Speaker::beep(int se_type, int t_ms) {
  switch (se_type) {
    case WAKEUP:
      beep(wakeup, sizeof(wakeup) / sizeof(float), t_ms);
      break;

    default:
      break;
  }
}

void Speaker::beep(float *mm, int m_size, int t_ms) {
  for (int i = 0; i < m_size; i++) {
    tone(sp, mm[i], t_ms);
  }
  noTone(sp);
}

void Speaker::noTone(int pin) {
  ledcWriteTone(LEDC_CHANNEL_2, 0.0);
}

void Speaker::tone(int pin, int freq, int t_ms) {
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(pin, LEDC_CHANNEL_2);
  ledcWriteTone(LEDC_CHANNEL_2, freq);

  delay(t_ms);
}
