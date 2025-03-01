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
    : mySpi(VSPI),
      ts(XPT2046_CS, XPT2046_IRQ),
      tft(SCREEN_WIDTH, SCREEN_HEIGHT)
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
  Serial.begin(9600);

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

  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
}

void TemplateCode::initializeLVGL()
{
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, nullptr, SCREEN_WIDTH * SCREEN_HEIGHT / 10);
}

void TemplateCode::setupTouchscreen()
{
  ts.begin(mySpi);
  ts.setRotation(1);
}

void TemplateCode::setupDisplay()
{
  tft.begin();
  tft.setRotation(3);

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
  uint16_t touchX = map(p.x, TOUCH_X_MIN, TOUCH_X_MAX, 1, SCREEN_WIDTH);
  uint16_t touchY = map(p.y, TOUCH_Y_MIN, TOUCH_Y_MAX, 1, SCREEN_HEIGHT);

  data->state = LV_INDEV_STATE_PR;
  data->point.x = touchX;
  data->point.y = touchY;
}

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
