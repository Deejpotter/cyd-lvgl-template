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
#ifdef TOUCH_TYPE_RESISTIVE
#include <XPT2046_Touchscreen.h>
#endif
#ifdef TOUCH_TYPE_CAPACITIVE
#include "CST820.h"
#endif
#include "RGBledDriver.h"

class TemplateCode
{

private:
  // Hardware Configuration
#ifdef TOUCH_TYPE_RESISTIVE
  static constexpr uint8_t XPT2046_IRQ = 36;
  static constexpr uint8_t XPT2046_MOSI = 13;
  static constexpr uint8_t XPT2046_MISO = 12;
  static constexpr uint8_t XPT2046_CLK = 14;
  static constexpr uint8_t XPT2046_CS = 33;
#endif
#ifdef TOUCH_TYPE_CAPACITIVE
  static constexpr uint8_t CST820_SDA = 33;
  static constexpr uint8_t CST820_SCL = 32;
  static constexpr uint8_t CST820_RST = 25;
  static constexpr uint8_t CST820_INT = 21;
#endif

  // Screen Configuration
  static constexpr uint16_t SCREEN_WIDTH = 320;
  static constexpr uint16_t SCREEN_HEIGHT = 240;

#ifdef TOUCH_TYPE_RESISTIVE
  // Touch Calibration Values
  static constexpr uint16_t TOUCH_X_MIN = 200;
  static constexpr uint16_t TOUCH_X_MAX = 3700;
  static constexpr uint16_t TOUCH_Y_MIN = 240;
  static constexpr uint16_t TOUCH_Y_MAX = 3800;
#endif

  // Hardware Instances
#ifdef TOUCH_TYPE_RESISTIVE
  SPIClass mySpi; // Reference to avoid copy
  XPT2046_Touchscreen ts;
#endif
#ifdef TOUCH_TYPE_CAPACITIVE
  CST820 ts;
#endif
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
  static void readTouchpad(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
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
  // Overload for resistive/capacitive handled in .cpp

// Debug functionality
#if LV_USE_LOG != 0
  static void debugPrint(const char *buf);
#endif

  // Periodic tasks
  void update();
};

#endif // TEMPLATE_CODE_H
