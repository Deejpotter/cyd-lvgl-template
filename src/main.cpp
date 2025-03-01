/**
 * Last updated: 1st Feb 2025
 * By: Daniel Potter
 * Description: This is the main file for the drawbot controller.
 * It contains the setup and loop functions and connects to the CNC shield through a serial connection to control the stepper motors etc.
 */

/**
 * --------- Includes ---------
 * Include the libraries here to keep eveything organised.
 */
// Includes the code from the original template to set up the screen and touch etc.
#include "TemplateCode.h"
// Includes the code to control the CNC shield.
#include "GRBLController.h"
// Import the main interface code for the UI.
#include "MainInterface.h"

/**
 * --------- Global variables ---------
 * Declare any global variables here that need to be accessed from multiple functions.
 */
// The reference to the singleton instance
TemplateCode &templateCode = TemplateCode::getInstance();
GRBLController grbl(Serial2); // Using Serial2 for GRBL communication
MainInterface mainInterface(grbl);

/**
 * --------- Custom user functions ---------
 * Add any custom functions here so the main loop and setup functions are kept clean and easy to read.
 */

/**
 * ------------------
 * Setup fuction
 * ------------------
 * The setup function is called once when the program starts. It is used to initialize the program and prepare it for running.
 * Add initialization code here that should run only once at the start of the program.
 */
void setup()
{
  // Initialize the template code.
  if (!templateCode.begin())
  {
    Serial.println("Failed to initialize template code.!");
    while (1)
    {
      delay(1000);
    } // Halt if initialization fails
  }

  // Initialize GRBL communication
  grbl.begin();

  // Initialize test interface
  mainInterface.init();

  /* Add custom setup code here. */

  Serial.println("Setup done");
}

/**
 * ------------------
 * Main loop
 * ------------------
 * The main loop of the program. This function is called repeatedly by the Arduino framework.
 * Add logic here that should run continuously but be careful not to add blocking code that will slow down the rest of the program.
 */
void loop()
{
  // Rin the update logic the template code. (this includes LVGL handling)
  templateCode.update();

  /* Add custom loop code here. */

  // Update the test interface each loop.
  mainInterface.update();
}
