//* Template for using Squareline Studio ui output with
//*   Cheap Yellow Display ("CYD") (aka ESP32-2432S028R)
//*
//*

#include <Arduino.h>
#include <SPI.h>
#include "RGBledDriver.h" // The custom functions for controlling the RGB LED.
#include <MobaTools.h>    // For the non-blocking stepper motor and button logic.
#include <Preferences.h>  // Used for persistent storage of settings and other information that should be saved between reboots.
#include <ezButton.h>     // For handling the limit switch.

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
#include <XPT2046_Touchscreen.h>

// A library for interfacing with the touch screen
//
// Can be installed from the library manager (Search for "XPT2046")
// https://github.com/PaulStoffregen/XPT2046_Touchscreen
// ----------------------------
// Touch Screen pins
// ----------------------------
// The CYD touch uses some non default
// SPI pins
#define XPT2046_IRQ 36
#define XPT2046_MOSI 13
#define XPT2046_MISO 12
#define XPT2046_CLK 14
#define XPT2046_CS 33

// Motor setup
#define STEP_PIN 22                // GPIO pin for Step Pulse
#define DIR_PIN 21                 // GPIO pin for Direction
MoToStepper stepper(200, STEPDIR); // 200 steps per revolution in step/direction mode

// Motor control variables
float motorSpeed = 200000; // Default speed ( steps / 10sec)
float motorAcc = 500;      // higher is longer ramp up time, too low motor will stall at high steps per
bool motorRunning = false;
bool reverse = false;

// Limit Switch Setup
#define LIMIT_SWITCH_PIN 35 // GPIO pin for the limit switch

// Use ezButton library to handle the limit switch.
ezButton limitSwitch(LIMIT_SWITCH_PIN);

// Power On/Off
#define ONOFF_PIN 1 // on CYD is U0TXD on P1 TX NOTE: Serial debug will no longer work

// SPIClass mySpi = SPIClass(HSPI); // touch does not work with this setting
SPIClass mySpi = SPIClass(VSPI); // critical to get touch working

XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

/*Change to your screen resolution*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/**
 * Persistent settings structure.
 * Add properties here that should be saved between reboots.
 */
struct Settings
{
    int32_t travelDistance;
    uint16_t maxSpeed;
    uint16_t acceleration;
};

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = (ts.tirqTouched() && ts.touched()); // this is the version needed for XPT2046 touchscreen
    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        // the following three lines are specific for using the XPT2046 touchscreen
        TS_Point p = ts.getPoint();
        touchX = map(p.x, 200, 3700, 1, screenWidth);  /* Touchscreen X calibration */
        touchY = map(p.y, 240, 3800, 1, screenHeight); /* Touchscreen X calibration */
        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

// Limit switch function to reverse motor direction when triggered
void SWLimit()
{
    reverse = !reverse;
}

void setup()
{
    //   Serial.begin(115200); /* prepare for possible serial debug */
    //   String LVGL_Arduino = "Hello Arduino! ";
    //   LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    //   Serial.println(LVGL_Arduino);
    //   Serial.println("I am LVGL_Arduino");

    // Set CYD LED to off
    initRGBled();
    ChangeRGBColor(RGB_COLOR_0);
    lv_init();
#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif
    mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
    ts.begin(mySpi);                                                  /* Touchscreen init */
    ts.setRotation(1);                                                /* Landscape orientation */
    tft.begin();                                                      /* TFT init */
    tft.setRotation(3);                                               // Landscape orientation  1 =  CYC usb on right, 2 for vertical SAB: 3= Landscape +180
                                                                      //   tft.invertDisplay(1);                                             // SAB needed to invert display
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    /* Uncomment to create simple label */
    // lv_obj_t *label = lv_label_create( lv_scr_act() );
    // lv_label_set_text( label, "Hello Ardino and LVGL!");
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
    ui_init();

    // Stepper motor configuration
    stepper.attach(STEP_PIN, DIR_PIN);
    stepper.setSpeedSteps(motorSpeed); // Initial speed
    stepper.setRampLen(motorAcc);      // Set ramp length for smooth starts/stops

    // Set GPIO to output to control the MOSFET
    pinMode(ONOFF_PIN, OUTPUT);

    // Initialize limit switch
    limitSwitch.setDebounceTime(50); // 50 ms debounce time

    //    Serial.println("Setup done");
}

void loop()
{

    // Run the stepper motor if enabled
    if (motorRunning)
    {
        stepper.rotate(reverse ? -1 : 1); // Rotate in the current direction
    }
    else
    {
        stepper.rotate(0); // Stop rotating when motor is off
    }

    // Update the limit switch state
    limitSwitch.loop();
    if (limitSwitch.isPressed())
    {
        SWLimit(); // Call the limit switch function when triggered
    }

    lv_label_set_text_fmt(ui_SpeedLabel, "%d", stepper.getSpeedSteps()); //  steps / 10sec
    lv_label_set_text_fmt(ui_PositionLabel, "%d", stepper.currentPosition());

    lv_timer_handler(); /* let the GUI do its work */
}

// Button callback functions as defined in SquareLine Studio

// Start Button Function
void stepperGo(lv_event_t *e)
{
    motorRunning = true;
    stepper.setSpeedSteps(motorSpeed); //  steps / 10sec
}

// Stop Button Function
void stepperStop(lv_event_t *e)
{
    motorRunning = false;
}

// Reverse Direction Button Function
void stepperRev(lv_event_t *e)
{
    reverse = !reverse;
}

// Speed Slider Function
void stepperSpeed(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    motorSpeed = lv_slider_get_value(slider); // Update motor speed from slider position ( steps / 10sec)
    stepper.setSpeedSteps(motorSpeed);        //  steps / 10sec
}

// Stepper Power on/off
void StepperPower_ON(lv_event_t *e)
{
    digitalWrite(ONOFF_PIN, HIGH);
    //    delay(200);
    //    motorRunning = true;
}
void StepperPower_OFF(lv_event_t *e)
{
    digitalWrite(ONOFF_PIN, LOW);
    motorRunning = false;
}
