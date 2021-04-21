// Reference:
// https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
#ifndef __CANSAT_SD_H__
#define __CANSAT_SD_H__

#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

class CanSatSd {
 public:
  CanSatSd();

  void readFile(fs::FS &fs, const char *path);
  void writeFile(fs::FS &fs, const char *path, const char *message);
  void appendFile(fs::FS &fs, const char *path, const char *message);
  void deleteFile(fs::FS &fs, const char *path);
};

#endif  // __CANSAT_SD_H__
