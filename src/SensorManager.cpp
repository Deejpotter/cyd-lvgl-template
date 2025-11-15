#include "SensorManager.h"
#include <DHT.h>
#include <Arduino.h>

SensorManager::SensorManager(uint8_t dhtPin, uint8_t dhtType, uint32_t intervalMs)
    : pin(dhtPin), type(dhtType), interval(intervalMs), lastRead(0), tmp(NAN), hum(NAN), dht(nullptr)
{
}

void SensorManager::begin()
{
  if (!dht)
  {
    dht = new DHT(pin, type);
    dht->begin();
  }
}

void SensorManager::update()
{
  if (!dht)
  {
    dht = new DHT(pin, type);
    dht->begin();
  }
  unsigned long now = millis();
  if ((uint32_t)(now - lastRead) < interval)
    return;
  lastRead = now;

  float t = dht->readTemperature();
  float h = dht->readHumidity();

  // Only sample when values are valid
  if (!isnan(t))
    tmp = t;
  if (!isnan(h))
    hum = h;

  // If we have a callback and values changed significantly - fire it
  if (cb)
  {
    bool changed = false;
    if (isnan(lastTemperature()) || fabs(tmp - lastTemperature()) >= 0.1f)
      changed = true;
    if (isnan(lastHumidity()) || fabs(hum - lastHumidity()) >= 0.1f)
      changed = true;
    if (changed)
      cb(tmp, hum);
  }
}

void SensorManager::onChange(Callback c)
{
  cb = c;
}

float SensorManager::lastTemperature() const { return tmp; }
float SensorManager::lastHumidity() const { return hum; }
