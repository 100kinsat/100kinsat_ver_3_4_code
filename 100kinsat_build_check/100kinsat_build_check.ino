#include <TinyGPS++.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "cansat_status.hpp"
#include "mpu9250.h"
#include "speaker.hpp"

static const uint8_t button = 35;
static const uint8_t led = 2;

volatile int state = ST_LED;
Speaker sp = Speaker();

TinyGPSPlus gps;
HardwareSerial ss(2);

Mpu9250 imu(&Wire, 0x68);

const int motorA[3] = {4, 13, 25};   // AIN1, AIN2, PWMA
const int motorB[3] = {14, 27, 26};  // BIN1, BIN2, PWMB

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;

const int LEDC_TIMER_BIT = 8;
const int LEDC_BASE_FREQ = 490;

void setup() {
  // シリアル通信の初期化
  Serial.begin(115200);
  Serial.println("***100kinSATのプログラム開始***");
  Serial.println("100kinSATのスイッチをクリックしてモードを切り替えます．");
  // GPSとの通信の初期化
  ss.begin(9600);

  // SDカードの初期化
    if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // タクトスイッチとLED
  pinMode(button, INPUT);
  attachInterrupt(button, switch_state, FALLING);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // モーター
  for (int i = 0; i < 3; i++) {
    pinMode(motorA[i], OUTPUT);
    pinMode(motorB[i], OUTPUT);
  }
  ledcSetup(CHANNEL_A, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_B, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(motorA[2], CHANNEL_A);
  ledcAttachPin(motorB[2], CHANNEL_B);

  // 9軸センサ
  if (!imu.Begin()) {
    Serial.println("9軸センサの初期化に失敗しました");
  }

  sp.beep(sp.WAKEUP, 150);
}

void loop() {
  switch (state) {
    case ST_LED:
      Serial.println("\r\n***LEDモード***");
      Serial.println("100kinSATのLEDが点滅を繰り返します．");
      toggle_led(500);
      break;

    case ST_GPS:
      Serial.println("\r\n***GPSセンサモード***");
      Serial.println("GPSセンサから緯度・経度の値を取得して表示します．");
      Serial.println("値が表示されない場合は，GPSセンサが衛星を補足しているか確認してください．");
      Serial.println("衛星を補足したらGPSセンサのLEDが点滅します．");
      display_gps_value();
      break;

    case ST_IMU:
      Serial.println("\r\n***9軸センサモード***");
      Serial.println("9軸センサの値を表示します．");
      display_imu_value();
      break;

    case ST_SD:
      Serial.println("\r\n***SDカードモード***");
      read_write_sd();
      break;

    case ST_MOTOR:
      Serial.println("\r\n***モータモード***");
      motor_test(100, 5000);
      motor_test(200, 5000);
      break;

    default:
      break;
  }
}

/**
 * @brief モード切替用割込み関数
 */
void switch_state() {
  state++;
  state = state % ST_END;
}

/**
 * @brief Lチカ
 */
void toggle_led(int t_ms) {
  while (state == ST_LED) {
    digitalWrite(led, HIGH);
    delay(t_ms);
    digitalWrite(led, LOW);
    delay(t_ms);
  }
}

/**
 * @brief GPSセンサから緯度・経度を取得して表示する
 */
void display_gps_value() {
  while (state == ST_GPS) {
    while (ss.available() > 0) {
      char c = ss.read();
      gps.encode(c);
      if (gps.location.isUpdated()) {
        Serial.printf("[GPS] 緯度: %9.6f\t", gps.location.lat());
        Serial.printf("経度: %9.6f\r\n", gps.location.lng());
      }
    }
  }
}

/**
 * @brief 9軸センサから値を取得して表示する
 */
void display_imu_value() {
  while (state == ST_IMU) {
    if (imu.Read()) {
      Serial.printf("[加速度] X: %+8.4f Y: %+8.4f Z: %+8.4f\t\t", imu.accel_x_mps2(),
                    imu.accel_y_mps2(), imu.accel_z_mps2());
      Serial.printf("[ジャイロ] X: %+8.4f Y: %+8.4f Z: %+8.4f\t\t", imu.gyro_x_radps(),
                    imu.gyro_y_radps(), imu.gyro_z_radps());
      Serial.printf("[地磁気] X: %+8.4f Y: %+8.4f Z: %+8.4f\n", imu.mag_x_ut(), imu.mag_y_ut(),
                    imu.mag_z_ut());
    }
  }
}

/**
 * @brief SDカードの動作確認
 */
void read_write_sd() {
  writeFile(SD, "/cansat_test.txt", "Hello ");
  appendFile(SD, "/cansat_test.txt", "CanSat!\n");
  while (state == ST_SD) {
  }
}

/**
 * SDカードに新規書き込みする
 */
 void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

/**
 * SDカードに追記する
 */
void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

/**
 * @brief モータの動作テスト
 */
void motor_test(int pwm, int t_ms) {
  while (state == ST_MOTOR) {
    // 前進
    Serial.printf("[前進] PWM: %d, Delay: %d ms\n", pwm, t_ms);
    // 左モータ（CCW，反時計回り）
    digitalWrite(motorA[1], LOW);
    digitalWrite(motorA[0], HIGH);
    ledcWrite(CHANNEL_A, pwm);
    // 右モータ（CW，時計回り）
    digitalWrite(motorB[1], LOW);
    digitalWrite(motorB[0], HIGH);
    ledcWrite(CHANNEL_B, pwm);

    delay(t_ms);

    // 停止
    Serial.printf("[停止] Delay: %d ms\n", t_ms);
    // 左モータ停止
    digitalWrite(motorA[0], LOW);
    digitalWrite(motorA[1], LOW);
    ledcWrite(CHANNEL_A, HIGH);
    // 右モータ停止
    digitalWrite(motorB[0], LOW);
    digitalWrite(motorB[1], LOW);
    ledcWrite(CHANNEL_B, HIGH);

    delay(t_ms);

    // 後退
    Serial.printf("[後退] PWM: %d, Delay: %d ms\n", pwm, t_ms);
    // 左モータ（CW，時計回り）
    digitalWrite(motorA[0], LOW);
    digitalWrite(motorA[1], HIGH);
    ledcWrite(CHANNEL_A, pwm);
    // 右モータ（CCW，反時計回り）
    digitalWrite(motorB[0], LOW);
    digitalWrite(motorB[1], HIGH);
    ledcWrite(CHANNEL_B, pwm);

    delay(t_ms);

    // 停止
    Serial.printf("[停止] Delay: %d ms\n", t_ms);
    // 左モータ停止
    digitalWrite(motorA[0], LOW);
    digitalWrite(motorA[1], LOW);
    ledcWrite(CHANNEL_A, HIGH);
    // 右モータ
    digitalWrite(motorB[0], LOW);
    digitalWrite(motorB[1], LOW);
    ledcWrite(CHANNEL_B, HIGH);

    delay(t_ms);
  }
}
