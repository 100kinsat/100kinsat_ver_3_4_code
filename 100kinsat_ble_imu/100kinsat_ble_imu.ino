#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "mpu9250.h"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

Mpu9250 imu(&Wire, 0x68);

static const uint8_t led = 2;

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      Serial.print("value: ");
      for (int i = 0; i < value.length(); i++) {
        Serial.print(value[i]);
      }
      Serial.println();
      if (value[0] == 'h') {
        digitalWrite(led, HIGH);
      } else if (value[0] == 'l') {
        digitalWrite(led, LOW);
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("BLE開始");

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  BLEDevice::init("100kinSAT");

  if (!imu.Begin()) {
    Serial.println("IMUセンサの初期化に失敗しました");
    while (1) {}
  }
}

void loop() {
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
    );
  pCharacteristic->setCallbacks(new MyCallbacks());

  float ax, ay, az = 0;
  String message = "";

  while (1) {
    if (imu.Read()) {
      ax = imu.accel_x_mps2();
      ay = imu.accel_y_mps2();
      az = imu.accel_z_mps2();

      message = String("ax: ") + String(ax) + String(" ay: ") + String(ay) + String(" az: ") + String(az);
      Serial.println(message);

      pCharacteristic->setValue(message.c_str());
      pService->start();
      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->addServiceUUID(SERVICE_UUID);
      pAdvertising->setScanResponse(true);
      pAdvertising->setMinPreferred(0x06);
      pAdvertising->setMinPreferred(0x12);
      BLEDevice::startAdvertising();
    }

    delay(2000);
  }
}
