# CYD LVGL Template

## Overview

This template supports JC2432W328R (Cheap Yellow Display), JC2432W328C, and JC4827W543R ESP32 boards. It initializes the display, touch screen, and LVGL interface directly in code (no Squareline Studio).

UI is built using LVGL directly in `src/`. Upload code to your board using PlatformIO.

Add your own logic, events, and UI elements in `src/` to create a custom project.

Use the pinouts and configuration in this template to connect sensors, motors, and other devices to the ESP32.

## Hardware Requirements

- JC2432W328R ESP32 Board
  - 2.8-inch TFT display (320x240 resolution)
  - ST7789 V2 display driver
  - ESP32-WROOM microcontroller
  - Built-in USB-C connector
  - Resistive touch screen

- JC2432W328C ESP32 Board
  - 2.8-inch TFT display (320x240 resolution)
  - ST7789 V2 display driver
  - ESP32-WROOM microcontroller
  - Built-in USB-C connector
  - Capacitive touch screen (CST820 controller)

- JC4827W543R ESP32 Board
  - 4.3-inch TFT display (480x272 resolution)
  - NV3041A display driver over 4-bit QSPI (ESP32-S3)
  - ESP32-S3-WROOM-1 module
  - Built-in USB-C connector
  - Resistive touch (XPT2046) or Capacitive touch (GT911) variants

## Development Environment Setup

## Required Software

- Visual Studio Code
- PlatformIO IDE Extension
- LVGL

## Libraries

```ini
lib_deps = 
  bodmer/TFT_eSPI@^2.5.42
  https://github.com/PaulStoffregen/XPT2046_Touchscreen.git
  lvgl/lvgl@^8.3.6
  https://github.com/bitbank2/bb_captouch.git
```

```txt
project/
├── src/
│   └── main.cpp           # Main program logic + LVGL UI
├── platformio.ini        # PlatformIO configuration
├── scripts/               # Pre-build template copy script
│   └── copy_template.py
├── template files/        # LVGL & TFT_eSPI config templates
│   ├── lv_conf.h
│   └── <env>/User_Setup.h
└── README.md
```

## Display Pin Configurations

### JC2432W328R

```cpp
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  -1
#define TFT_BL   27
// Touch (resistive)
#define TOUCH_CS 33
#define TOUCH_IRQ 36
```

> Note: The pinout above is confirmed for the JC2432W328R (resistive). Some online sources mention pins 21 and 27 may be switched on other models (like the JC2432S028R); this template uses the configuration from the [env:jc2432w328r] in `platformio.ini`.

### JC2432W328C

```cpp
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  -1
#define TFT_BL   27
// Touch (capacitive, CST820)
#define I2C_SDA 33
#define I2C_SCL 32
#define TOUCH_INT 21
// Optional reset
#define CST820_RST 25
```

### JC4827W543R (ESP32-S3 + NV3041A QSPI, resistive XPT2046) — confirmed

Display (NV3041A over QSPI):

```cpp
// QSPI data bus to panel
#define QSPI_CS  45
#define QSPI_SCK 47
#define QSPI_D0  21
#define QSPI_D1  48
#define QSPI_D2  40
#define QSPI_D3  39

// Backlight (active HIGH)
#define GFX_BL   1
```

Touch (XPT2046 on SPI):

```cpp
#define XPT2046_CS   38
#define XPT2046_IRQ  3
#define XPT2046_MOSI 11
#define XPT2046_MISO 13
#define XPT2046_CLK  12
// Optional calibration (defaults work for most panels)
#define TOUCH_X_MIN 200
#define TOUCH_X_MAX 3700
#define TOUCH_Y_MIN 240
#define TOUCH_Y_MAX 3800
```

Notes:

- Environment: `env:jc4827w543r` sets `board = esp32-s3-devkitc-1` (ESP32-S3) and uses Arduino_GFX only for this board (other boards remain on TFT_eSPI).
- Driver: Arduino_GFX with `Arduino_ESP32QSPI` + `Arduino_NV3041A`.
- Backlight: GPIO1, active HIGH.
- Orientation: default `TFT_ROTATION=0` (change in `platformio.ini` if needed).
- Color inversion: NV3041A needs INVON (0x21) for correct colors on most JC4827 modules; Arduino_GFX or a custom init can send this.

## Models and Orientation

This template supports the boards via per-environment build flags. Primary differences are touch interfaces and pinouts:

- JC2432W328R (resistive): XPT2046 (SPI)
- JC2432W328C (capacitive): CST820 (I2C)
- JC4827W543R (resistive): XPT2046 (SoftSPI)

Model selection and orientation are controlled via build flags per environment in `platformio.ini`:

- `MODEL_JC2432W328R` or `MODEL_JC2432W328C` (set by the selected environment)
- `TFT_ROTATION` selects the screen orientation for both display and touch

Rotation values (same as TFT_eSPI):

- 0: Portrait (USB at bottom)
- 1: Landscape CW
- 2: Portrait 180°
- 3: Landscape CCW

Defaults in this repo (`platformio.ini`):

- Default env: `jc2432w328c`
- `env:jc2432w328r`: `-DMODEL_JC2432W328R -DTFT_ROTATION=1`
- `env:jc2432w328c`: `-DMODEL_JC2432W328C -DTFT_ROTATION=1`
- `env:jc4827w543r`: `-DMODEL_JC4827W543R -DTFT_ROTATION=0`

Change orientation by editing `TFT_ROTATION` in the environment you build.

## Building and Flashing

### 1. Open in VS Code with PlatformIO

Open the project in Visual Studio Code with the PlatformIO extension installed.

Templates are copied automatically before build.

The pre-build script `scripts/copy_template.py` copies the needed files from `template files/` into `.pio/libdeps/<env>/`:

- `template files/lv_conf.h` -> `.pio/libdeps/<env>/lv_conf.h`
- `template files/<env>/User_Setup.h` -> `.pio/libdeps/<env>/TFT_eSPI/User_Setup.h`

You don’t need to move these files manually.

### 2. Build the selected environment

```bash
pio run -e <env>
# examples:
pio run -e jc2432w328r
pio run -e jc2432w328c
pio run -e jc4827w543r
```

### 3. Upload to the device

```bash
pio run -e <env> --target upload
```

## UI Modifications

Build your UI directly with LVGL in `src/`. No Squareline Studio workflow is used. Add or modify event handlers and logic in your own `.cpp` files as needed.

### Important Notes

- All custom logic should go in `main.cpp` or separate files
- Keep the main loop running smoothly for responsive UI
- No scheduler or sensor manager are included by default; the loop is kept minimal

## Troubleshooting

Common issues and solutions:

### Display Issues

Ensure correct display driver (ST7789_2) is selected
Verify pin configurations match the hardware

### Build Issues

Check PlatformIO.ini configuration
Verify library versions are compatible

## Resources

### JC2432W328R links

- Almost correct pinout (pins 21 and 27 are switched): [ESP32 Cheap Yellow Display (CYD) Pinout (ESP32-2432S028R) | Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-cheap-yellow-display-cyd-pinout-esp32-2432s028r/#speaker)
- This specific model of device (not quite accurate): <https://github.com/maxpill/JC2432W328>
- Cheap yellow display github: <https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/tree/main>
- Link to Squarline CYD template: <https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/discussions/102>
- Reddit discussion also for this specific model of device: <https://www.reddit.com/r/esp32/comments/1dy5k11/working_cyd_jc2432w328_display_240x320_28_usbc/?rdt=34968>
- ESP32 Marauder CYD port: maxpill/ESP32-Marauder-JC2432W328-Cheap-Yellow-Display: JC2432W328 ESP32Marauder Cheap Yellow Display
- Case 1: [GUITION JC2432W328C cover by Matej's Workshop | Download free STL model | Printables.com](https://www.printables.com/model/913023-guition-jc2432w328c-cover/files)
- Case 2: [ESP32 2.8inch JC2432W328 Case by GrafMax17 - Thingiverse](https://www.thingiverse.com/thing:6892431)

### JC2432W328C links

- Working touch driver: [GitHub - bitbank2/bb_captouch: Capacitive touch library for ESP32 using FocalTech CST820 controller](https://github.com/bitbank2/bb_captouch)
- Official/Community documentation: [maxpill/JC2432W328 GitHub](https://github.com/maxpill/JC2432W328)
- Community discussion and working code: [Reddit: Working CYD JC2432W328 Display 240x320 2.8" USB-C](https://www.reddit.com/r/esp32/comments/1dy5k11/working_cyd_jc2432w328_display_240x320_28_usbc/)
- Product listing and specs: [AliExpress JC2432W328C](https://aliexpress.com/item/1005006729707613.html)
- ST7789 display driver datasheet: [ST7789 PDF](https://www.rhydolabz.com/documents/33/ST7789.pdf)
- CST820 touch controller info: [CST820 datasheet (PDF)](https://datasheet.lcsc.com/lcsc/1811141810_FocalTech-Systems-CST820S_C181837.pdf)

### JC4827W543R links

- Guition/JCZN hardware models (translated): <https://www.guition.com/doc_en/>
- Profi-max JC4827W543 examples: <https://github.com/profi-max/JC4827W543_4.3inch_ESP32S3_board>
- CNCWiki community page: <https://www.cncwiki.org/index.php?title=JCZN_JC4827W543_ESP32_4.3%22_Touch_Screen_Display_Module>
- calint notes/examples: <https://github.com/calint/JC4827W543R>
- vldmr-d Guition ESP32-S3 repo: <https://github.com/vldmr-d/Guition-ESP32-S3-JC4827W543R_I>
- NV3041A driver datasheet: <https://admin.osptek.com/uploads/NV_3041_A_Datasheet_V1_2_20221011_686486a221.pdf>
- ESP32-S3-WROOM-1 datasheet: <https://www.espressif.com>

Confirmed pinout and init reference:

- Calint’s QSPI NV3041A bring-up (includes gamma and INVON sequence):
  - Repo: <https://github.com/calint/platformio-bam>
  - Device file: <https://github.com/calint/platformio-bam/blob/master/src/devices/JC4827W543.hpp>
    - Resistive touch variant: <https://raw.githubusercontent.com/calint/platformio-bam/refs/heads/master/src/devices/JC4827W543R.hpp>
    - Capacitive touch variant: <https://raw.githubusercontent.com/calint/platformio-bam/refs/heads/master/src/devices/JC4827W543C.hpp>

## How template files are applied

Template files are copied automatically before build by the pre-build script (`scripts/copy_template.py`).

- `template files/lv_conf.h` -> `.pio/libdeps/<env>/lv_conf.h`
- `template files/<env>/User_Setup.h` -> `.pio/libdeps/<env>/TFT_eSPI/User_Setup.h`

No manual copying is required.
