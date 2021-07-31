const int motorA[3] = {13, 4, 25};  // AIN1, AIN2, PWMA
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
  // 前進
  // 左モータ（CCW，反時計回り）
  digitalWrite(motorA[1], LOW);
  digitalWrite(motorA[0], HIGH);
  ledcWrite(CHANNEL_A, 80);

  // 右モータ（CW，時計回り）
  digitalWrite(motorB[1], LOW);
  digitalWrite(motorB[0], HIGH);
  ledcWrite(CHANNEL_B, 80);
  
  delay(1000);

  // 停止
  // 左モータ停止
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], LOW);
  ledcWrite(CHANNEL_A, HIGH);

  // 右モータ停止
  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], LOW);
  ledcWrite(CHANNEL_B, HIGH);
  
  delay(1000);

  // 後退
  // 左モータ（CW，時計回り）
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], HIGH);
  ledcWrite(CHANNEL_A, 80);

  // 右モータ（CCW，反時計回り）
  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], HIGH);
  ledcWrite(CHANNEL_B, 80);
  
  delay(1000);

  // 停止
  // 左モータ停止
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], LOW);
  ledcWrite(CHANNEL_A, HIGH);

  // 右モータ
  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], LOW);
  ledcWrite(CHANNEL_B, HIGH);
  
  delay(1000);
}
