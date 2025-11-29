/**
 * Last updated: 2nd Feb 2025
 * By: Daniel Potter
 * Description: This file contains the template code for setting up the screen and touch interface.
 */

#include "TemplateCode.h"

// Initialize static members
TemplateCode *TemplateCode::instance = nullptr;
lv_disp_draw_buf_t TemplateCode::draw_buf;
lv_color_t TemplateCode::buf[SCREEN_WIDTH * SCREEN_HEIGHT / 10];

TemplateCode::TemplateCode()
#if defined(MODEL_JC2432W328R)
    : mySpi(VSPI),
      ts(XPT2046_CS, XPT2046_IRQ),
      tft(SCREEN_WIDTH, SCREEN_HEIGHT)
#elif defined(MODEL_JC2432W328C)
    : ts(),
      tft(SCREEN_WIDTH, SCREEN_HEIGHT)
#else
    : tft(SCREEN_WIDTH, SCREEN_HEIGHT)
#endif
{
}

TemplateCode &TemplateCode::getInstance()
{
  if (instance == nullptr)
  {
    instance = new TemplateCode();
  }
  return *instance;
}

bool TemplateCode::begin()
{
#if LV_USE_LOG != 0
  lv_log_register_print_cb(debugPrint);
#endif

  initializeHardware();
  initializeLVGL();
  setupTouchscreen();
  setupDisplay();

  return true;
}

void TemplateCode::initializeHardware()
{
  initRGBled();
  ChangeRGBColor(RGB_COLOR_1);

#if defined(MODEL_JC2432W328R)
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
#endif
}

void TemplateCode::initializeLVGL()
{
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, nullptr, SCREEN_WIDTH * SCREEN_HEIGHT / 10);
}

void TemplateCode::setupTouchscreen()
{
#if defined(MODEL_JC2432W328R)
  ts.begin(mySpi);
  ts.setRotation(TFT_ROTATION);
#endif
#if defined(MODEL_JC2432W328C)
  // Initialize BitBank capacitive touch with I2C pins + reset/irq
  ts.init(CST820_SDA, CST820_SCL, CST820_RST, CST820_INT);

  // Map TFT_ROTATION (0..3) to degrees for setOrientation
  int oriDeg = 0;
  switch (TFT_ROTATION & 0x3)
  {
  case 0:
    oriDeg = 0;
    break;
  case 1:
    oriDeg = 90;
    break;
  case 2:
    oriDeg = 180;
    break;
  case 3:
    oriDeg = 270;
    break;
  }
  ts.setOrientation(oriDeg, SCREEN_WIDTH, SCREEN_HEIGHT);
  Serial.printf("BBCapTouch init: sensor=%d orient=%d %ux%u\n", ts.sensorType(), oriDeg, (unsigned)SCREEN_WIDTH, (unsigned)SCREEN_HEIGHT);
  Serial.flush();
#endif
}

void TemplateCode::setupDisplay()
{
  tft.begin();
  tft.setRotation(TFT_ROTATION);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = flushDisplay;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = readTouchpad;
  lv_indev_drv_register(&indev_drv);
}

void TemplateCode::flushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  auto &display = getInstance();
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  display.tft.startWrite();
  display.tft.setAddrWindow(area->x1, area->y1, w, h);
  display.tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  display.tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

#if defined(MODEL_JC2432W328R)
void TemplateCode::readTouchpad(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  auto &display = getInstance();
  bool touched = (display.ts.tirqTouched() && display.ts.touched());

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
    return;
  }

  TS_Point p = display.ts.getPoint();
  // Map raw ADC coords into 0..(W-1) / 0..(H-1)
  int32_t mappedX = map(p.x, TOUCH_X_MIN, TOUCH_X_MAX, 0, SCREEN_WIDTH - 1);
  int32_t mappedY = map(p.y, TOUCH_Y_MIN, TOUCH_Y_MAX, 0, SCREEN_HEIGHT - 1);

  // Observation: this board's resistive wiring returns coordinates that are
  // inverted on both axes compared to the LV coordinate frame (top-left -> max,max).
  // Correct by inverting both axes so physical top-left -> (0,0) in LV.
  int32_t touchX = (SCREEN_WIDTH - 1) - mappedX;
  int32_t touchY = (SCREEN_HEIGHT - 1) - mappedY;

  // Clamp to valid ranges
  if (touchX < 0)
    touchX = 0;
  if (touchX > (int32_t)SCREEN_WIDTH - 1)
    touchX = SCREEN_WIDTH - 1;
  if (touchY < 0)
    touchY = 0;
  if (touchY > (int32_t)SCREEN_HEIGHT - 1)
    touchY = SCREEN_HEIGHT - 1;

  data->state = LV_INDEV_STATE_PR;
  data->point.x = (lv_coord_t)touchX;
  data->point.y = (lv_coord_t)touchY;

  // Throttled debug output for resistive touch (minimal CPU impact)
  {
    static uint32_t lastDbgR = 0;
    uint32_t now = millis();
    if (now - lastDbgR > 250)
    {
      lastDbgR = now;
      Serial.printf("XPT raw:%u,%u -> Mapped:%ld,%ld -> LV:%ld,%ld\n", p.x, p.y, (long)mappedX, (long)mappedY, (long)touchX, (long)touchY);
      Serial.flush();
    }
  }
}
#endif
#if defined(MODEL_JC2432W328C)
void TemplateCode::readTouchpad(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  auto &display = getInstance();
  TOUCHINFO ti = {};
  if (display.ts.getSamples(&ti) && ti.count > 0)
  {
    // BBCapTouch already applies setOrientation() so coords match LV frame
    int sx = (int)ti.x[0];
    int sy = (int)ti.y[0];
    if (sx < 0)
      sx = 0;
    if (sx >= (int)SCREEN_WIDTH)
      sx = SCREEN_WIDTH - 1;
    if (sy < 0)
      sy = 0;
    if (sy >= (int)SCREEN_HEIGHT)
      sy = SCREEN_HEIGHT - 1;
    data->state = LV_INDEV_STATE_PR;
    data->point.x = sx;
    data->point.y = sy;
    // Throttled minimal debug
    static uint32_t lastDbgC = 0;
    uint32_t now = millis();
    if (now - lastDbgC > 250)
    {
      lastDbgC = now;
      Serial.printf("BBCapTouch sample: %u -> LV:%d,%d\n", ti.count, data->point.x, data->point.y);
      Serial.flush();
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}
#endif

void TemplateCode::update()
{
  lv_timer_handler();
}

#if LV_USE_LOG != 0
void TemplateCode::debugPrint(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif
