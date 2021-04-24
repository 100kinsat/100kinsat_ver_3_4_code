static const uint8_t button = 35; // スイッチと接続しているピン（35番ピン）
static const uint8_t led = 2; // LEDと接続しているピン（2番ピン）

void setup() {
  pinMode(button, INPUT); // スイッチを入力モードに設定
  pinMode(led, OUTPUT); // LEDを出力モードに設定
  digitalWrite(led, LOW); // LEDへの出力をLOWにする（消灯）
}

void loop() {

  while (!digitalRead(button)) { // スイッチが押されている間
    digitalWrite(led, HIGH); // LEDへの出力をHIGHにする（点灯）
  }

  digitalWrite(led, LOW); // LEDへの出力をLOWにする（消灯）
}
