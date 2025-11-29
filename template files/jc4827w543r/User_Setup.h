//                            USER DEFINED SETTINGS
//   Set driver type, fonts to be loaded, pins used and SPI control method etc
//   This file is for JC4827W543R_I resistive touch board (4.3" 480x272)

#define ST7789_2_DRIVER
#define TFT_INVERSION_OFF
#define TFT_WIDTH 480
#define TFT_HEIGHT 272
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST -1
#define TFT_BL 1
#define TFT_BACKLIGHT_ON HIGH
#define TFT_BACKLIGHT_OFF LOW
#ifndef QSPI_CS
// QSPI pins for NV3041A (4.3" JC4827). Only define if not already set.
#define QSPI_CS 45 /* cs */
#define QSPI_SCK 47 /* sck */
#define QSPI_D0 21 /* d0 */
#define QSPI_D1 48 /* d1 */
#define QSPI_D2 40 /* d2 */
#define QSPI_D3 39 /* d3 */
#endif
#define TOUCH_CS 38 // XPT2046 CS (per JC4827 community mapping)
#define TOUCH_IRQ 3 // XPT2046 IRQ (per JC4827 community mapping)
// Use separate SoftSPI for XPT2046 to avoid bus contention (common JC4827 wiring)
#define TOUCH_MOSI 11
#define TOUCH_MISO 13
#define TOUCH_CLK 12
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT
#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 16000000
#define SPI_TOUCH_FREQUENCY 2500000
// Calibration values (override defaults if needed)
#define TOUCH_X_MIN 200
#define TOUCH_X_MAX 3700
#define TOUCH_Y_MIN 240
#define TOUCH_Y_MAX 3800
