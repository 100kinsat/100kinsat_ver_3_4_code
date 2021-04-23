#define mC 261.626 // ド
#define mD 293.665 // レ
#define mE 329.628 // ミ
#define mF 349.228 // ファ
#define mG 391.995 // ソ
#define mA 440.000 // ラ 
#define mB 493.883 // シ
#define nn 0.0

#define LEDC_CHANNEL_2 2
#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

static const uint8_t speaker = 12;

void setup() {
  pinMode(speaker, OUTPUT);

  // ド
  tone(speaker, mC * 2, 200);
  noTone(speaker);

  // レ
  tone(speaker, mD * 2, 200);
  noTone(speaker);

  // ミ
  tone(speaker, mE * 2, 200);
  noTone(speaker);

  // ファ
  tone(speaker, mF * 2, 200);
  noTone(speaker);

  // ソ
  tone(speaker, mG * 2, 200);
  noTone(speaker);

  // ラ
  tone(speaker, mA * 2, 200);
  noTone(speaker);

  // シ
  tone(speaker, mB * 2, 200);
  noTone(speaker);

  // ド
  tone(speaker, mC * 4, 200);
  noTone(speaker);
}

void loop() {
}

void tone(int pin, int freq, int t_ms) {
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(pin, LEDC_CHANNEL_2);
  ledcWriteTone(LEDC_CHANNEL_2, freq);

  delay(t_ms);
}

void noTone(int pin) {
  ledcWriteTone(LEDC_CHANNEL_2, 0.0);
}
