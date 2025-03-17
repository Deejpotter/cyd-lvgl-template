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
  /**
   * -------- UI Container Elements --------
   * Hierarchical structure of the interface:
   * mainScreen
   * ├── headerContainer (fixed at top)
   * └── scrollContainer (scrollable content area)
   */
  lv_obj_t *mainScreen;      // Primary screen container (320x240 portrait)
  lv_obj_t *headerContainer; // Fixed header area (40px height)
  lv_obj_t *scrollContainer; // Scrollable content area (remaining space)

  /**
   * -------- Display Elements --------
   * Text and data display widgets within containers
   */
  lv_obj_t *headerLabel; // Title text in header
  lv_obj_t *tempLabel;   // Temperature value display

  /**
   * -------- Private Helper Methods --------
   * Internal functions for UI component creation and management
   */
  void createHeader();        // Creates and configures the fixed header
  void createScrollContent(); // Creates the scrollable content area

public:
  /**
   * -------- Public Interface --------
   * Main class interface for external interaction
   */
  MainInterface();  // Constructor - initializes pointers
  ~MainInterface(); // Destructor - cleanup handled by LVGL

  void init();   // Initialize the complete interface
  void update(); // Update display (called in main loop)
};

#endif // MAIN_INTERFACE_H
