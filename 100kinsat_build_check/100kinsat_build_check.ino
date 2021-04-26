#include <TinyGPS++.h>

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

void setup() {
  Serial.begin(115200);
  Serial.println("***100kinSATのプログラム開始***");
  Serial.println("100kinSATのスイッチをクリックしてモードを切り替えます．");
  ss.begin(9600);

  pinMode(button, INPUT);
  attachInterrupt(button, switch_state, FALLING);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

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
      Serial.printf("[加速度] X: %+8.4f Y: %+8.4f Z: %+8.4f\t\t", imu.accel_x_mps2(), imu.accel_y_mps2(),
                    imu.accel_z_mps2());
      Serial.printf("[ジャイロ] X: %+8.4f Y: %+8.4f Z: %+8.4f\t\t", imu.gyro_x_radps(),
                    imu.gyro_y_radps(), imu.gyro_z_radps());
      Serial.printf("[地磁気] X: %+8.4f Y: %+8.4f Z: %+8.4f\n", imu.mag_x_ut(), imu.mag_y_ut(),
                    imu.mag_z_ut());
    }
  }
}
