#include <TinyGPS++.h>

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
HardwareSerial ss(2);

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println("lat,lng");
}

void loop() {
  while(ss.available() > 0){
    char c = ss.read();
    gps.encode(c);
    if(gps.location.isUpdated()){
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");
      Serial.println(gps.location.lng(), 6);
    }
  }
}
