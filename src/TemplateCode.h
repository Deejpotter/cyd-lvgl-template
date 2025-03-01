/**
 * Last updated: 2nd Feb 2025
 * By: Daniel Potter
 * Description: This file contains the template code for setting up the screen and touch interface.
 */

#ifndef TEMPLATE_CODE_H
#define TEMPLATE_CODE_H

#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "RGBledDriver.h"

class TemplateCode
{
private:
  // Hardware Configuration
  static constexpr uint8_t XPT2046_IRQ = 36;
  static constexpr uint8_t XPT2046_MOSI = 13;
  static constexpr uint8_t XPT2046_MISO = 12;
  static constexpr uint8_t XPT2046_CLK = 14;
  static constexpr uint8_t XPT2046_CS = 33;

  // Screen Configuration
  static constexpr uint16_t SCREEN_WIDTH = 320;
  static constexpr uint16_t SCREEN_HEIGHT = 240;

  // Touch Calibration Values
  static constexpr uint16_t TOUCH_X_MIN = 200;
  static constexpr uint16_t TOUCH_X_MAX = 3700;
  static constexpr uint16_t TOUCH_Y_MIN = 240;
  static constexpr uint16_t TOUCH_Y_MAX = 3800;

  // Hardware Instances
  SPIClass mySpi; // Reference to avoid copy
  XPT2046_Touchscreen ts;
  TFT_eSPI tft;

  // LVGL Buffer
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[SCREEN_WIDTH * SCREEN_HEIGHT / 10];

  // Singleton instance
  static TemplateCode *instance;

  // Private constructor for singleton
  TemplateCode();

  // Helper initialization methods
  void initializeHardware();
  void initializeLVGL();
  void setupTouchscreen();
  void setupDisplay();

public:
  // Delete copy constructor and assignment operator
  TemplateCode(const TemplateCode &) = delete;
  TemplateCode operator=(const TemplateCode &) = delete;

  // Singleton access
  static TemplateCode &getInstance();

  // Main initialization
  bool begin();

  // LVGL callback handlers
  static void flushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
  static void readTouchpad(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

// Debug functionality
#if LV_USE_LOG != 0
  static void debugPrint(const char *buf);
#endif

  // Periodic tasks
  void update();
};

#endif // TEMPLATE_CODE_H
