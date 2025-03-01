// FileManager.h
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>
#include <SD.h>

class FileManager
{
private:
  static const uint8_t SD_CS_PIN = 5;

public:
  bool begin()
  {
    return SD.begin(SD_CS_PIN);
  }

  bool openFile(const char *filename)
  {
    return SD.exists(filename);
  }
};
#endif
