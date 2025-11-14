// MainInterface.h
/**
 * MainInterface.h
 * Last Updated: March 17, 2025
 * Author: Daniel Potter
 *
 * Description:
 * This header file defines the MainInterface class which manages a simple
 * scrollable interface with temperature display. Designed for portrait orientation
 * with a fixed header and scrollable content area.
 *
 * LVGL Layout Concepts:
 * - Flex Layout: Modern flexible box layout system
 * - Scrolling: Native LVGL scrolling support
 * - Containers: Parent-child widget relationship
 *
 * UI/UX Principles:
 * - Fixed header for consistent navigation
 * - Scrollable content for expandability
 * - Clear visual hierarchy
 */

#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <lvgl.h>
#include <string>

using std::string;

class MainInterface
{

private:
  // UI Containers
  lv_obj_t *mainScreen;
  lv_obj_t *headerContainer;

  // Display Elements
  lv_obj_t *headerLabel;
  lv_obj_t *tempLabel;
  lv_obj_t *humidityLabel;

  // Helper Methods
  void createHeader();

public:
  MainInterface();
  ~MainInterface();

  void init();
  void update();

  // Methods to update sensor values
  void setTemperature(float tempC);
  void setHumidity(float humidity);
};

#endif // MAIN_INTERFACE_H
