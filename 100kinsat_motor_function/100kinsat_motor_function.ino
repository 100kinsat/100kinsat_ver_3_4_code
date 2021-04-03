const int motorA[3] = {4, 13, 25};  // AIN1, AIN2, PWMA
const int motorB[3] = {14, 27, 26}; // BIN1, BIN2, PWMB

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;

const int LEDC_TIMER_BIT = 8;
const int LEDC_BASE_FREQ = 490;

void setup() {
  for(int i = 0; i < 3; i++){
    pinMode(motorA[i], OUTPUT);
    pinMode(motorB[i], OUTPUT);
  }

  ledcSetup(CHANNEL_A, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_B, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

  ledcAttachPin(motorA[2], CHANNEL_A);
  ledcAttachPin(motorB[2], CHANNEL_B);
}

void loop() {
  ccw(80);
  delay(1000);

  stop();
  delay(1000);

  cw(80);
  delay(1000);

  stop();
  delay(1000);
}

/** 正転（反時計回り） */
void ccw(int pwm) {
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], HIGH);
  ledcWrite(CHANNEL_A, pwm);
  
  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], HIGH);
  ledcWrite(CHANNEL_B, pwm);
}

/** 逆転（時計回り） */
void cw(int pwm) {
  digitalWrite(motorA[1], LOW);
  digitalWrite(motorA[0], HIGH);
  ledcWrite(CHANNEL_A, pwm);
  
  digitalWrite(motorB[1], LOW);
  digitalWrite(motorB[0], HIGH);
  ledcWrite(CHANNEL_B, pwm);
}

/** 停止 */
void stop() {
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], LOW);
  ledcWrite(CHANNEL_A, HIGH);

  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], LOW);
  ledcWrite(CHANNEL_B, HIGH);
}
