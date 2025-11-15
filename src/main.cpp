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
// Import the main interface code for the UI.
#include "MainInterface.h"

#include <LovyanGFX.hpp> // Display library: https://github.com/lovyan03/LovyanGFX
#include "CST820.h"      // Custom I2C driver for CST820 capacitive touchscreen
#include "PeriodicScheduler.h"
#include "SensorManager.h"
#include <DHT.h>

/**
 * --------- Global variables ---------
 * Declare any global variables here that need to be accessed from multiple functions.
 */
// The reference to the singleton instance

TemplateCode &templateCode = TemplateCode::getInstance();
MainInterface mainInterface = MainInterface();

// DHT11 sensor setup (external sensor)
// Connect DHT11 data pin to GPIO21 or GPIO22 (choose one available)
// DHT11 sensor setup (external sensor)
// Connect DHT11 data pin to GPIO21 or GPIO22 (choose one available)
#define DHTPIN 21 // Change to 22 if needed
#define DHTTYPE DHT11

// Scheduler for periodic tasks
PeriodicScheduler scheduler;

// Manager for sensors - DHT operations are abstracted here
SensorManager sensorManager(DHTPIN, DHTTYPE, 2000);

/**
 * --------- Custom user functions ---------
 * Add any custom functions here so the main loop and setup functions are kept clean and easy to read.
 */

// ====== Custom Display Class for ST7789 TFT ======
class LGFX_JustDisplay : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7789 _panel; // ST7789 panel instance
  lgfx::Bus_SPI _bus;        // SPI bus instance

public:
  LGFX_JustDisplay(void)
  {
    { // SPI bus configuration
      auto cfg = _bus.config();
      cfg.spi_host = VSPI_HOST;  // Use VSPI (default high-speed SPI)
      cfg.spi_mode = 0;          // SPI mode 0
      cfg.freq_write = 27000000; // Write speed: 27 MHz
      cfg.freq_read = 16000000;  // Read speed (not used here)
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1; // Use DMA channel 1
      cfg.pin_sclk = 14;   // Clock pin
      cfg.pin_mosi = 13;   // Data out (MOSI)
      cfg.pin_miso = -1;   // No data in
      cfg.pin_dc = 2;      // Data/command pin
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    { // Display panel configuration
      auto cfg = _panel.config();
      cfg.pin_cs = 15;          // Chip select
      cfg.pin_rst = -1;         // No reset pin
      cfg.pin_busy = -1;        // Not used
      cfg.panel_width = 240;    // Native width
      cfg.panel_height = 320;   // Native height
      cfg.offset_rotation = 0;  // No offset
      cfg.dummy_read_pixel = 8; // For compatibility
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = false; // No inversion
      cfg.rgb_order = 0;  // RGB (not BGR)
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};

// Create display and touch controller instances
LGFX_JustDisplay tft;
CST820 touch(33, 32, 25, 21); // Touch: SDA, SCL, RST, INT

/**
 * ------------------
 * Setup fuction
 * ------------------
 * The setup function is called once when the program starts. It is used to initialize the program and prepare it for running.
 * Add initialization code here that should run only once at the start of the program.
 */
void setup()
{
  // Initialize DHT sensor via SensorManager
  sensorManager.begin();

  // Initialize the template code.
  if (!templateCode.begin())
  {
    Serial.println("Failed to initialize template code.!");
    while (1)
    {
      delay(1000);
    } // Halt if initialization fails
  }

  // Initialize the main interface
  mainInterface.init();

  // Register callback to update UI when sensors change
  sensorManager.onChange([&](float t, float h)
                         {
    if (!isnan(t)) mainInterface.setTemperature(t);
    if (!isnan(h)) mainInterface.setHumidity(h); });

  // Schedule sensor reads and UI updates
  scheduler.addTask(std::bind(&SensorManager::update, &sensorManager), 2000);
  scheduler.addTask(std::bind(&MainInterface::update, &mainInterface), 100);

  /* Add custom setup code here. */

  // Initialize I2C for CST820 (if not already done in CST820::begin)
  // Wire.begin(I2C_SDA, I2C_SCL); // CST820::begin() should handle this

  // Initialize touchscreen
  touch.begin();

  // Debug: Read CST820 chip ID
  uint8_t chip_id = touch.readChipID();
  Serial.print("CST820 Chip ID: 0x");
  Serial.println(chip_id, HEX);

  Serial.begin(115200);
  delay(500);
  Serial.println("🧪 Touch + Display test starting...");

  // Enable backlight (GPIO 27 must be HIGH)
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 100);
  tft.println("Touch to draw");

  Serial.println("✅ Setup complete");
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

  // Run the update logic for the template code (includes LVGL handling)
  templateCode.update();

  // Sensor reads are handled by SensorManager registered with the PeriodicScheduler

  // Scheduler handles periodic sensor reads and UI updates
  scheduler.update();

  delay(10); // Small delay - scheduler controls update frequency
}
