#pragma once

#include <functional>
#include <stdint.h>
class DHT;

class SensorManager
{
public:
  using Callback = std::function<void(float tempC, float humidity)>;

  SensorManager(uint8_t dhtPin, uint8_t dhtType, uint32_t intervalMs = 2000);
  void begin();
  void update(); // immediate read; intended to be called by scheduler

  void onChange(Callback cb);

  float lastTemperature() const;
  float lastHumidity() const;

private:
  uint8_t pin;
  uint8_t type;
  uint32_t interval;
  uint32_t lastRead;
  float tmp;
  float hum;
  Callback cb;
  DHT *dht;
};
