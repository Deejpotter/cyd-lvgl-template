# CYD LVGL Template

## Overview

This is a template that can be used to create a project for the JC2432W328R (AKA the Cheap Yellow Display) ESP32 board. The project is designed to initialize the display, touch screen, and lvgl interface.
The user interface is built using LVGL. The code is uploaded to the CYD using PlatformIO.

The rest of the project is up to you! You can add your own logic, events, and UI elements to create a custom project.

Use the pins and other information in this template to connect sensors, motors, and other devices to the ESP32.

## Hardware Requirements

- JC2432W328R ESP32 Board
  - 2.8-inch TFT display (320x240 resolution)
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

## Display Pin Configuration

```cpp
# define TFT_MISO 12
# define TFT_MOSI 13
# define TFT_SCLK 14
# define TFT_CS   15
# define TFT_DC   2
# define TFT_RST  -1
# define TFT_BL   27
```

Display Pin Configuration

```cpp
# define TFT_MISO 12
# define TFT_MOSI 13
# define TFT_SCLK 14
# define TFT_CS   15
# define TFT_DC   2
# define TFT_RST  -1
# define TFT_BL   27
```

``` txt
Available GPIO for Actuator Control
GPIO21 (I2C SDA)
GPIO22 (I2C SCL)
GPIO35 (Input only, analog)
GPIO17, 16, 4 (RGB LED pins, can be repurposed)
```

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

The user interface is built using Squareline Studio. To modify:

- Open the project in Squareline Studio
- Make desired changes
- Export to the src/ui directory
- Implement any new event handlers in ui_events.cpp

### Important Notes

- Don't modify the generated UI files directly

- All custom logic should go in main.cpp or separate files
- Always implement events in ui_events.cpp
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

- Almost correct pinout (pins 21 and 27 are switched): [ESP32 Cheap Yellow Display (CYD) Pinout (ESP32-2432S028R) | Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-cheap-yellow-display-cyd-pinout-esp32-2432s028r/#speaker)
- This specific model of device (not quite accurate): <https://github.com/maxpill/JC2432W328>
- Squareline: <https://squareline.io/downloads>
- Cheap yellow display github: <https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/tree/main>
- Link to Squarline CYD template: <https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/discussions/102>
- Reddit discussion also for this specific model of device: <https://www.reddit.com/r/esp32/comments/1dy5k11/working_cyd_jc2432w328_display_240x320_28_usbc/?rdt=34968>
- ESP32 Marauder CYD port: maxpill/ESP32-Marauder-JC2432W328-Cheap-Yellow-Display: JC2432W328 ESP32Marauder Cheap Yellow Display
- Case 1: [GUITION JC2432W328C cover by Matej's Workshop | Download free STL model | Printables.com](https://www.printables.com/model/913023-guition-jc2432w328c-cover/files)
- Case 2: [ESP32 2.8inch JC2432W328 Case by GrafMax17 - Thingiverse](https://www.thingiverse.com/thing:6892431)

## Original Template Readme

Extract the zip to a folder where you wish to keep your new project.
(You may want to keep a folder just for the template and duplicate that for each new project).
In Visual Studio Code with Platformio, open the template folder.
When you first open the folder containing this template, platformio will load the necessary libraries.

This template includes a simple project from Squareline Studio ("SLS")
  It is already in the src/ui directory.  
  You can build and upload this project to your CYD as a test if you like.
  The project has a blue button.  When it is pressed, the ball will drop and bounce off the button.

You will be exporting your SLS ui into that directory AFTER
  first deleting all the files in that directory.

## To insert (export) a project from SLS

### 1. First set your export parameters in File/Project Settings

``` txt
Width:320
Height: 240
Depth: 16-bit
Board Group: Arduino
Board: Arduino with TFT_eSPI
```

### 2. Set UI Files Export Path to the path to your projects's src/ui folder

for example:
  C:\Users\owner\Documents\PlatformIO\Projects\Squareline Studio CYD Base Project\src\ui\filelist.txt

### 3. Set Call functions export file to .cpp

### 4. click on APPLY CHANGES

### 5. Note the LVGL version here and make sure you platfomio.ini lib-dep references the same version

for example:

``` txt
lib_deps = 
  bodmer/TFT_eSPI@^2.5.42
  https://github.com/PaulStoffregen/XPT2046_Touchscreen.git ;#v1.4
  lvgl/lvgl@^8.3.6 ; align this rev with the rev used by Squareline Studio
```

### 6. Then to actually export the SLS project into your platformio project, click Export/Export UI Files

### 7. Next you have to copy files to different locations.  These files are in the directory "NECESARY TEMPLATE FILES"

``` txt
copy lv_conf.h to .pio\libdeps\cyd\lv_conf.h
copy User_Setup.h to .pio\libdeps\cyd\TFT_eSPI
(these paths are noted in the comment at the top of each file)
```

Check that all of the fonts you used in the SLS project are turned on.  These start at line 366 of lv_conf.h
   "1" indicates that you need the font, "0" indicates that you do not need the font.

DO NOT move the lvgl examples directory into your src folder.  In fact, you can delete this and the demos directory
 if you wish.  The compilation of these directories is already turned off in lv_conf.h

In main.cpp, confirm the rotation of the display matches your project design.  line 126 of main.cpp
       tft.setRotation(1); // Landscape orientation  1 =  CYC usb on right, 2 for vertical

You should now be able to build your project and further edit the main.cpp code to add necessary real-world
  interfaces, etc.
