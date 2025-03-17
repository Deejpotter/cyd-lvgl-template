// MainInterface.cpp
/**
 * MainInterface.cpp
 * Last Updated: March 17, 2025
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
  scrollContainer = nullptr;
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
  createScrollContent();

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
void MainInterface::createScrollContent()
{
  // Create main scrollable container
  scrollContainer = lv_obj_create(mainScreen);
  // Set size to fill remaining space (320-40 height)
  lv_obj_set_size(scrollContainer, 240, 280);
  // Match main screen background
  lv_obj_set_style_bg_color(scrollContainer, lv_color_hex(0x000000), LV_PART_MAIN);
  // Add padding for content spacing
  lv_obj_set_style_pad_all(scrollContainer, 10, 0);

  // Configure scrolling behavior
  // Enable vertical scrolling only
  lv_obj_set_scroll_dir(scrollContainer, LV_DIR_VER);
  // Enable snap-to-center scrolling
  lv_obj_set_scroll_snap_y(scrollContainer, LV_SCROLL_SNAP_CENTER);

  // Create temperature display widget
  tempLabel = lv_label_create(scrollContainer);
  // Use larger font for better readability
  lv_obj_set_style_text_font(tempLabel, &lv_font_montserrat_28, 0);
  // Set initial text with placeholder value
  lv_label_set_text(tempLabel, "Temperature:\n0.0°C");
  // Center align at top of scroll area
  lv_obj_align(tempLabel, LV_ALIGN_TOP_MID, 0, 20);
}

/**
 * Update function called in main loop
 * Will be implemented later with sensor data
 */
void MainInterface::update()
{
  // Future implementation:
  // - Read sensor data
  // - Update temperature display
  // - Handle any UI animations
}
