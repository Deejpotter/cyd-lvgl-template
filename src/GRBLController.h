/**
 * Last updated: 2nd Feb 2025
 * By: Daniel Potter
 * Description: This file contains the GRBL command logic for the drawbot controller.
 * It should handle all communication with the GRBL controller and provide an interface for sending commands and receiving status updates.
 */

#ifndef GRBL_CONTROLLER_H
#define GRBL_CONTROLLER_H

#include <Arduino.h> // This includes Stream and uint8_t definitions

class GRBLController
{
private:
  static const int SERIAL_BAUD = 9600;
  HardwareSerial &serial; // Changed from Stream to HardwareSerial
  String responseBuffer;
  bool debugMode = true; // Toggle for debug messages

public:
  GRBLController(HardwareSerial &serialPort) : serial(serialPort) {}

  void begin()
  {
    serial.begin(SERIAL_BAUD);
    if (debugMode)
      Serial.println("GRBL: Initializing connection...");

    // Send wake up command
    serial.println("\r\n\r\n");
    delay(500); // Wait for GRBL to initialize

    // Clear any startup messages
    while (serial.available())
    {
      String response = serial.readStringUntil('\n');
      if (debugMode)
        Serial.println("GRBL Startup: " + response);
    }
  }

  // Send a command and print debug info
  void sendCommand(const char *cmd)
  {
    if (debugMode)
      Serial.printf("GRBL Sending: %s\n", cmd);
    serial.println(cmd);

    // Wait for and print immediate response
    String response = waitForResponse();
    if (debugMode)
      Serial.printf("GRBL Response: %s\n", response.c_str());
  }

  // Get current status with debug info
  String getStatus()
  {
    if (debugMode)
      Serial.println("GRBL: Requesting status...");
    serial.write('?'); // GRBL status query command
    String status = waitForResponse();
    if (debugMode)
      Serial.printf("GRBL Status: %s\n", status.c_str());
    return status;
  }

  // Wait for response with timeout
  String waitForResponse()
  {
    String response = "";
    unsigned long timeout = millis() + 1000; // 1 second timeout

    while (millis() < timeout)
    {
      if (serial.available())
      {
        char c = serial.read();
        response += c;
        if (c == '\n')
          break;
      }
    }

    if (millis() >= timeout && debugMode)
    {
      Serial.println("GRBL: Response timeout!");
    }

    return response;
  }

  // Simple test command to verify communication
  void testCommunication()
  {
    if (debugMode)
      Serial.println("GRBL: Testing communication...");

    // Send a simple command that should return 'ok'
    sendCommand("G21"); // Set to millimeters
    delay(100);

    // Request current status
    getStatus();
  }

  // Toggle debug output
  void setDebug(bool enable)
  {
    debugMode = enable;
  }
};
#endif