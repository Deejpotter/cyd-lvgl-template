# CYD LVGL Template

## Overview

This is a template that can be used to create a project for the JC2432W328R (AKA the Cheap Yellow Display) ESP32 board, as well as the JC2432W328C and JC4827W543R. The project is designed to initialize the display, touch screen, and lvgl interface.
The user interface is built using LVGL. The code is uploaded to the CYD using PlatformIO.

The rest of the project is up to you! You can add your own logic, events, and UI elements to create a custom project.

Use the pins and other information in this template to connect sensors, motors, and other devices to the ESP32.

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
  - 4.82-inch TFT display (540x480 resolution)
  - ST7789 V2 display driver
  - ESP32-WROOM microcontroller
  - Built-in USB-C connector

## Development Environment Setup

## Required Software

- Visual Studio Code
- PlatformIO IDE Extension
- LVGL

## Libraries

```ini
lib_deps = 
   bodmer/TFT_eSPI@^2.5.42
   lvgl/lvgl@^8.3.6
```

```txt
project/
├── src/
│   ├── main.cpp           # Main program logic
│   ├── ui/               # Generated UI files from Squareline
│   │   ├── ui.h
│   │   ├── ui.c
│   │   ├── ui_events.cpp  # Custom event handlers
│   │   └── ui_events.h
│   └── config/           # Configuration files
├── platformio.ini        # PlatformIO configuration
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
#define TOUCH_IRQ 39
```

> Note: The pinout above is confirmed for the JC2432W328R (resistive). Some online sources mention pins 21 and 27 may be switched on other models; this template uses the configuration from the [env:jc2432w328r] in `platformio.ini`.

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
#define TOUCH_SDA 18
#define TOUCH_SCL 19
#define TOUCH_INT 39
```

> Note: The pinout above is typical for the JC2432W328C (capacitive, CST820). The CST820 touch controller uses I2C. Confirm your board's silkscreen or schematic if you encounter issues.

### JC4827W543R

## Building and Flashing

### 1. Clone the repository

```bash
git clone git@github.com:Makerstore/actuator-controller.git
```

### 2. Open in VS Code with PlatformIO

Open the project in Visual Studio Code with the PlatformIO extension installed.

After installing the required libraries, some files need to be moved to make the project build correctly.

The lv_conf.h file needs to be moved from the template folder to sit NEXT to the lvgl folder in the lib folder.

The User_Setup.h file needs to be moved to the TFT_eSPI folder in the lib folder to replace the existing file.

### 3. Build the project

```bash
pio run
```

### 4. Upload to the device

```bash
pio run --target upload
```

## UI Modifications

The user interface is built using the provided template files. To modify or extend the UI, edit the template source files in the `src/` directory. Implement any new event handlers or logic in your own `.cpp` files as needed.

### Important Notes

- All custom logic should go in `main.cpp` or separate files
- Keep the main loop running smoothly for responsive UI

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
- Squareline: <https://squareline.io/downloads>
- Cheap yellow display github: <https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/tree/main>
- Link to Squarline CYD template: <https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/discussions/102>
- Reddit discussion also for this specific model of device: <https://www.reddit.com/r/esp32/comments/1dy5k11/working_cyd_jc2432w328_display_240x320_28_usbc/?rdt=34968>
- ESP32 Marauder CYD port: maxpill/ESP32-Marauder-JC2432W328-Cheap-Yellow-Display: JC2432W328 ESP32Marauder Cheap Yellow Display
- Case 1: [GUITION JC2432W328C cover by Matej's Workshop | Download free STL model | Printables.com](https://www.printables.com/model/913023-guition-jc2432w328c-cover/files)
- Case 2: [ESP32 2.8inch JC2432W328 Case by GrafMax17 - Thingiverse](https://www.thingiverse.com/thing:6892431)

### JC2432W328C links

- Official/Community documentation: [maxpill/JC2432W328 GitHub](https://github.com/maxpill/JC2432W328)
- Community discussion and working code: [Reddit: Working CYD JC2432W328 Display 240x320 2.8" USB-C](https://www.reddit.com/r/esp32/comments/1dy5k11/working_cyd_jc2432w328_display_240x320_28_usbc/)
- Product listing and specs: [AliExpress JC2432W328C](https://aliexpress.com/item/1005006729707613.html)
- ST7789 display driver datasheet: [ST7789 PDF](https://www.rhydolabz.com/documents/33/ST7789.pdf)
- CST820 touch controller info: [CST820 datasheet (PDF)](https://datasheet.lcsc.com/lcsc/1811141810_FocalTech-Systems-CST820S_C181837.pdf)

### JC4827W543R links

## Original Template Readme

Extract the zip to a folder where you wish to keep your new project.
(You may want to keep a folder just for the template and duplicate that for each new project).
In Visual Studio Code with Platformio, open the template folder.
When you first open the folder containing this template, platformio will load the necessary libraries.

## Template File Usage

The template files provided in this repository are designed to help you quickly set up a working LVGL project for supported ESP32 display boards. Use the files in the `template files/` directory as a starting point for your own project.

### Steps

1. Copy the necessary template files (such as `main.cpp`, `lv_conf.h`, `User_Setup.h`, etc.) from the `template files/` directory into your project as needed.
2. Adjust pin assignments and configuration in `platformio.ini` and the template files to match your hardware.
3. Build and upload the project using PlatformIO.

Refer to comments in the template files for further customization and extension.
