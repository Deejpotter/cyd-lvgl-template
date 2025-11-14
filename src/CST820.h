// Credit to https://github.com/pay191/DIY_Malls-JC2432W328C_Tests for this code.

#ifndef CST820_H
#define CST820_H

#include <Wire.h>

// ====== CST820 Capacitive Touchscreen Driver ======
// Handles initialization and I2C-based touch reading for CST820

class CST820
{
public:
  // Constructor: pass in the I2C and GPIO pins used
  CST820(uint8_t sda, uint8_t scl, uint8_t rst, uint8_t irq)
      : _sda(sda), _scl(scl), _rst(rst), _irq(irq) {}

  // Initialize the touch controller
  void begin()
  {
    // Reset sequence for CST820
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, LOW);  // Hold reset low
    delay(10);                // Wait a moment
    digitalWrite(_rst, HIGH); // Release reset
    delay(100);               // Wait for controller to boot

    // Start I2C on the provided SDA/SCL pins
    Wire.begin(_sda, _scl);
  }

  // Optional: Read chip ID from CST820 for verification
  uint8_t readChipID()
  {
    Wire.beginTransmission(0x15); // CST820 I2C address
    Wire.write(0xA7);             // Register 0xA7 is the Chip ID register
    Wire.endTransmission(false);
    if (Wire.requestFrom(0x15, 1) != 1)
      return 0xFF;
    return Wire.read(); // Should return 0xB7 for CST820
  }

  // Read current touch point (if any)
  bool getTouch(uint16_t *x, uint16_t *y, uint8_t *gesture = nullptr)
  {
    Wire.beginTransmission(0x15); // I2C address for CST820
    Wire.write(0x00);             // Start reading from register 0 (touch data)

    // If failed to request 7 bytes, exit
    if (Wire.endTransmission(false) != 0 || Wire.requestFrom(0x15, 7) != 7)
    {
      return false;
    }

    // Read all 7 bytes of touch event data
    uint8_t buf[7];
    for (int i = 0; i < 7; i++)
      buf[i] = Wire.read();

    // Number of touches (low nibble of buf[2])
    uint8_t touches = buf[2] & 0x0F;
    if (touches == 0)
      return false; // No touch detected

    // Optional: store gesture byte if pointer provided
    if (gesture)
      *gesture = buf[1];

    // Decode X/Y coordinates from MSB/LSB format
    *x = ((buf[3] & 0x0F) << 8) | buf[4]; // X = high 4 bits + LSB
    *y = ((buf[5] & 0x0F) << 8) | buf[6]; // Y = high 4 bits + LSB

    return true;
  }

private:
  // Pin assignments for this instance
  uint8_t _sda, _scl, _rst, _irq;
};

#endif