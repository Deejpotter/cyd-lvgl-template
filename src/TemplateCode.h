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
#if defined(MODEL_JC4827W543R) && defined(ENABLE_ARDUINO_GFX)
// Forward declarations to avoid pulling full Arduino_GFX umbrella header
class Arduino_DataBus;
class Arduino_NV3041A;
class Arduino_GFX;
#endif
#if !defined(MODEL_JC4827W543R) || !defined(ENABLE_ARDUINO_GFX)
#include <TFT_eSPI.h>
#endif
#if defined(MODEL_JC2432W328R) || defined(MODEL_JC4827W543R)
#include <XPT2046_Touchscreen.h>
#elif defined(MODEL_JC2432W328C)
#include <bb_captouch.h>
#endif
#include "RGBledDriver.h"

// Pin mapping via PlatformIO build flags (prefer PIO-defined macros)
// Resistive (XPT2046) pins
#if defined(MODEL_JC2432W328R) || defined(MODEL_JC4827W543R)
#ifndef XPT2046_CS
#ifdef TOUCH_CS
#define XPT2046_CS TOUCH_CS
#else
#define XPT2046_CS 33
#endif
#endif
#ifndef XPT2046_IRQ
#ifdef TOUCH_IRQ
#define XPT2046_IRQ TOUCH_IRQ
#else
#define XPT2046_IRQ 36
#endif
#endif
#ifndef XPT2046_CLK
#if defined(TOUCH_CLK)
#define XPT2046_CLK TOUCH_CLK
#elif defined(TFT_SCLK)
#define XPT2046_CLK TFT_SCLK
#else
#define XPT2046_CLK 14
#endif
#endif
#ifndef XPT2046_MISO
#if defined(TOUCH_MISO)
#define XPT2046_MISO TOUCH_MISO
#elif defined(TFT_MISO)
#define XPT2046_MISO TFT_MISO
#else
#define XPT2046_MISO 12
#endif
#endif
#ifndef XPT2046_MOSI
#if defined(TOUCH_MOSI)
#define XPT2046_MOSI TOUCH_MOSI
#elif defined(TFT_MOSI)
#define XPT2046_MOSI TFT_MOSI
#else
#define XPT2046_MOSI 13
#endif
#endif
#endif // MODEL_JC2432W328R || MODEL_JC4827W543R

// Capacitive (CST820) pins
#if defined(MODEL_JC2432W328C)
#ifndef CST820_SDA
#ifdef I2C_SDA
#define CST820_SDA I2C_SDA
#else
#define CST820_SDA 33
#endif
#endif
#ifndef CST820_SCL
#ifdef I2C_SCL
#define CST820_SCL I2C_SCL
#else
#define CST820_SCL 32
#endif
#endif
#ifndef CST820_RST
#define CST820_RST 25
#endif
#ifndef CST820_INT
#define CST820_INT 21
#endif
#endif // MODEL_JC2432W328C

// The BitBank `BBCapTouch` library handles a variety of controllers and
// provides rotation support via `setOrientation()`. No raw-range macros
// are required when using that library.

class TemplateCode
{

private:
  // Hardware Configuration
  // Pins are provided via macros from PlatformIO (see above mapping).

  // Screen Configuration (per touch/display orientation)
  // LVGL expects width x height.
#ifndef TFT_ROTATION
#define TFT_ROTATION 0
#endif

  // Native panel resolution per model
#if defined(MODEL_JC4827W543R)
  static constexpr uint16_t PANEL_WIDTH = 480;
  static constexpr uint16_t PANEL_HEIGHT = 272;
#else
  // ST7789 on CYD: 240x320 (portrait)
  static constexpr uint16_t PANEL_WIDTH = 240;
  static constexpr uint16_t PANEL_HEIGHT = 320;
#endif

// Derive LVGL resolution from rotation: 0/2 -> portrait, 1/3 -> landscape
#if (TFT_ROTATION == 0) || (TFT_ROTATION == 2)
  static constexpr uint16_t SCREEN_WIDTH = PANEL_WIDTH;
  static constexpr uint16_t SCREEN_HEIGHT = PANEL_HEIGHT;
#else
  static constexpr uint16_t SCREEN_WIDTH = PANEL_HEIGHT;
  static constexpr uint16_t SCREEN_HEIGHT = PANEL_WIDTH;
#endif

#if defined(MODEL_JC2432W328R) || defined(MODEL_JC4827W543R)
// Touch Calibration Values (overridable via PlatformIO build flags)
// Define TOUCH_X_MIN, TOUCH_X_MAX, TOUCH_Y_MIN, TOUCH_Y_MAX in platformio.ini to customize.
// Defaults match common JC2432W328R panels.
#ifndef TOUCH_X_MIN
#define TOUCH_X_MIN 200
#endif
#ifndef TOUCH_X_MAX
#define TOUCH_X_MAX 3700
#endif
#ifndef TOUCH_Y_MIN
#define TOUCH_Y_MIN 240
#endif
#ifndef TOUCH_Y_MAX
#define TOUCH_Y_MAX 3800
#endif
#endif

  // Hardware Instances
#if defined(MODEL_JC2432W328R) || defined(MODEL_JC4827W543R)
  SPIClass mySpi; // Reference to avoid copy
  XPT2046_Touchscreen ts;
#endif
#if defined(MODEL_JC2432W328C)
  BBCapTouch ts;
#endif
#if defined(MODEL_JC4827W543R) && defined(ENABLE_ARDUINO_GFX)
  Arduino_DataBus *qspiBus;
  Arduino_NV3041A *panel;
  Arduino_GFX *gfxCanvas; // Arduino_Canvas
#else
  TFT_eSPI tft;
#endif

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
