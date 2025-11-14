/**
 * Last Updated: 14/11/25
 * Author: Daniel Potter
 *
 * Description:
 * Implements a simple scrollable interface with temperature display.
 * Optimized for portrait orientation (320x240).
 *
 * LVGL Basics:
 * - lv_obj_t: Base type for all LVGL widgets
 * - Styles: Control appearance of objects
 * - Flex Layout: Modern flexible box layout system
 *
 * Layout Concepts:
 * - Fixed header: Always visible at top
 * - Scrollable content: Vertically scrollable area below header
 * - Flex layout: Automatic vertical arrangement of elements
 *
 * UI/UX Principles:
 * - Clear visual hierarchy
 * - Consistent spacing
 * - Readable typography
 */

#include "MainInterface.h"
#include <stdio.h>

/**
 * Constructor: Initializes all UI element pointers to nullptr
 * This prevents undefined behavior if accessed before initialization
 */
MainInterface::MainInterface()
{
  mainScreen = nullptr;
  headerContainer = nullptr;
  headerLabel = nullptr;
  tempLabel = nullptr;
}

/**
 * Destructor: Cleanup handled automatically by LVGL
 * LVGL's parent-child relationship manages memory
 */
MainInterface::~MainInterface()
{
  // LVGL handles cleanup through parent-child relationships
}

/**
 * Main initialization function
 * Creates and configures all UI elements in the proper hierarchy
 */
void MainInterface::init()
{
  // Create main screen container
  mainScreen = lv_obj_create(NULL);
  // Set portrait orientation (240x320)
  lv_obj_set_size(mainScreen, 240, 320);
  // Set dark theme background
  lv_obj_set_style_bg_color(mainScreen, lv_color_hex(0x000000), LV_PART_MAIN);
  // Remove default padding
  lv_obj_set_style_pad_all(mainScreen, 0, 0);

  // Configure flex layout for vertical arrangement
  // Similar to CSS flexbox with column direction
  lv_obj_set_layout(mainScreen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(mainScreen, LV_FLEX_FLOW_COLUMN);

  // Create UI components in order (top to bottom)
  createHeader();

  // Create temperature display directly on mainScreen
  tempLabel = lv_label_create(mainScreen);
  lv_obj_set_style_text_font(tempLabel, &lv_font_montserrat_28, 0);
  lv_obj_set_style_text_color(tempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(tempLabel, "Temperature:\n--.-°C");
  lv_obj_align(tempLabel, LV_ALIGN_TOP_MID, 0, 50);

  // Create humidity display directly on mainScreen
  humidityLabel = lv_label_create(mainScreen);
  lv_obj_set_style_text_font(humidityLabel, &lv_font_montserrat_28, 0);
  lv_obj_set_style_text_color(humidityLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_label_set_text(humidityLabel, "Humidity:\n--.-%");
  lv_obj_align_to(humidityLabel, tempLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

  // Activate the screen
  lv_scr_load(mainScreen);
}

/**
 * Creates the fixed header area
 * Header contains title and remains at top of screen
 */
void MainInterface::createHeader()
{
  // Create header container with fixed height
  headerContainer = lv_obj_create(mainScreen);
  lv_obj_set_size(headerContainer, 240, 40);

  // Style header with dark theme
  lv_obj_set_style_bg_color(headerContainer, lv_color_hex(0x1E1E1E), LV_PART_MAIN);
  lv_obj_set_style_pad_all(headerContainer, 5, 0);

  // Create and configure header text
  headerLabel = lv_label_create(headerContainer);
  lv_label_set_text(headerLabel, "Temperature Monitor");
  // Center align the header text
  lv_obj_align(headerLabel, LV_ALIGN_CENTER, 0, 0);
  // Set text color to white for contrast
  lv_obj_set_style_text_color(headerLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
}

/**
 * Creates the scrollable content area
 * This area fills the remaining space below header
 * and enables vertical scrolling
 */

/**
 * Update function called in main loop
 * Will be implemented later with sensor data
 */

void MainInterface::update()
{
  // Placeholder: update logic if needed
}

void MainInterface::setTemperature(float tempC)
{
  char buf[32];
  snprintf(buf, sizeof(buf), "Temperature:\n%.1f°C", tempC);
  lv_label_set_text(tempLabel, buf);
}

void MainInterface::setHumidity(float humidity)
{
  char buf[32];
  snprintf(buf, sizeof(buf), "Humidity:\n%.1f%%", humidity);
  lv_label_set_text(humidityLabel, buf);
}
