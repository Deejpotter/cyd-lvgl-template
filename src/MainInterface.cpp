/**
 * Last Updated: Nov 29, 2025
 * Author: Daniel Potter
 *
 * Generic display and touch test UI:
 * - Header with resolution and model
 * - Color bars + gradient box
 * - Touch marker and (optional) trail
 * - Bouncing square + FPS indicator
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
  infoLabel = nullptr;
  fpsLabel = nullptr;
  colorRow = nullptr;
  gradientBox = nullptr;
  bouncer = nullptr;
  touchMarker = nullptr;
  for (int i = 0; i < TRAIL_CAP; ++i)
    trail[i] = nullptr;
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
  // Create main screen
  mainScreen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(mainScreen, lv_color_hex(0x000000), 0);
  lv_obj_set_style_pad_all(mainScreen, 4, 0);

  // Flex column layout
  lv_obj_set_layout(mainScreen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(mainScreen, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(mainScreen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

  // Header + tests
  createHeader();
  createTests();

  // Touch event on screen (pressed/pressing)
  lv_obj_add_event_cb(mainScreen, screenTouchEvent, LV_EVENT_PRESSED, this);
  lv_obj_add_event_cb(mainScreen, screenTouchEvent, LV_EVENT_PRESSING, this);
  lv_obj_add_event_cb(mainScreen, screenTouchEvent, LV_EVENT_RELEASED, this);

  // Load screen
  lv_scr_load(mainScreen);
}

/**
 * Creates the fixed header area
 * Header contains title and remains at top of screen
 */
void MainInterface::createHeader()
{
  uint16_t w = lv_disp_get_hor_res(NULL);
  uint16_t h = lv_disp_get_ver_res(NULL);

  headerContainer = lv_obj_create(mainScreen);
  lv_obj_remove_style_all(headerContainer);
  lv_obj_set_size(headerContainer, w, 42);
  lv_obj_set_style_bg_color(headerContainer, lv_color_hex(0x1E1E1E), 0);
  lv_obj_set_style_pad_all(headerContainer, 6, 0);

  headerLabel = lv_label_create(headerContainer);
  lv_obj_set_style_text_color(headerLabel, lv_color_hex(0xFFFFFF), 0);

#if defined(MODEL_JC2432W328R)
  const char *model = "JC2432W328R (Resistive)";
#elif defined(MODEL_JC2432W328C)
  const char *model = "JC2432W328C (Capacitive)";
#else
  const char *model = "CYD";
#endif
  char buf[96];
  lv_snprintf(buf, sizeof(buf), "%s  |  %ux%u", model, (unsigned)w, (unsigned)h);
  lv_label_set_text(headerLabel, buf);
  lv_obj_align(headerLabel, LV_ALIGN_LEFT_MID, 0, 0);

  // Info + FPS labels below header
  infoLabel = lv_label_create(mainScreen);
  lv_obj_set_style_text_color(infoLabel, lv_color_hex(0xA0A0A0), 0);
  lv_label_set_text(infoLabel, "Touch: --,-  state: idle");
  lv_obj_align(infoLabel, LV_ALIGN_TOP_LEFT, 4, 48);

  fpsLabel = lv_label_create(mainScreen);
  lv_obj_set_style_text_color(fpsLabel, lv_color_hex(0xA0FFA0), 0);
  lv_label_set_text(fpsLabel, "FPS: --");
  lv_obj_align(fpsLabel, LV_ALIGN_TOP_RIGHT, -4, 48);
}

void MainInterface::createColorBars()
{
  colorRow = lv_obj_create(mainScreen);
  lv_obj_remove_style_all(colorRow);
  lv_obj_set_width(colorRow, lv_pct(100));
  lv_obj_set_height(colorRow, 36);
  lv_obj_set_layout(colorRow, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(colorRow, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_gap(colorRow, 4, 0);

  auto make_box = [&](lv_color_t c)
  {
    lv_obj_t *b = lv_obj_create(colorRow);
    lv_obj_remove_style_all(b);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_style_bg_color(b, c, 0);
    return b;
  };

  make_box(lv_color_hex(0xFF0000)); // R
  make_box(lv_color_hex(0x00FF00)); // G
  make_box(lv_color_hex(0x0000FF)); // B
  make_box(lv_color_hex(0xFFFFFF)); // W
  make_box(lv_color_hex(0x000000)); // K

  // Gradient box
  gradientBox = lv_obj_create(mainScreen);
  lv_obj_remove_style_all(gradientBox);
  lv_obj_set_width(gradientBox, lv_pct(100));
  lv_obj_set_height(gradientBox, 24);
  static lv_style_t grad;
  static bool inited = false;
  if (!inited)
  {
    inited = true;
    lv_style_init(&grad);
    lv_style_set_bg_grad_dir(&grad, LV_GRAD_DIR_HOR);
    lv_style_set_bg_color(&grad, lv_color_hex(0x000000));
    lv_style_set_bg_grad_color(&grad, lv_color_hex(0xFFFFFF));
  }
  lv_obj_add_style(gradientBox, &grad, 0);
}

void MainInterface::createTests()
{
  createColorBars();

  // Bouncing square
  bouncer = lv_obj_create(mainScreen);
  lv_obj_remove_style_all(bouncer);
  lv_obj_set_size(bouncer, 20, 20);
  lv_obj_set_style_bg_color(bouncer, lv_color_hex(0x00A0FF), 0);
  lv_obj_set_style_radius(bouncer, 4, 0);
  lv_obj_align(bouncer, LV_ALIGN_TOP_LEFT, bx, by);

  // Touch marker
  touchMarker = lv_obj_create(mainScreen);
  lv_obj_remove_style_all(touchMarker);
  lv_obj_set_size(touchMarker, 10, 10);
  lv_obj_set_style_bg_color(touchMarker, lv_color_hex(0xFFD000), 0);
  lv_obj_set_style_radius(touchMarker, LV_RADIUS_CIRCLE, 0);
  lv_obj_add_flag(touchMarker, LV_OBJ_FLAG_HIDDEN);
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
  // Bouncer animation within screen bounds (below header and info rows)
  uint16_t w = lv_disp_get_hor_res(NULL);
  uint16_t h = lv_disp_get_ver_res(NULL);

  // Compute available area: margin below fps/info ~ 80px
  int top_margin = 80;
  int maxx = (int)w - 20 - 4;
  int maxy = (int)h - 20 - 4;
  if (bx <= 0 || bx >= maxx)
    bdx = -bdx;
  if (by <= top_margin || by >= maxy)
    bdy = -bdy;
  bx += bdx;
  by += bdy;
  if (by < top_margin)
    by = top_margin;
  lv_obj_set_pos(bouncer, bx, by);

  updateFPS();
}

void MainInterface::updateFPS()
{
  static uint32_t last = 0;
  static uint16_t frames = 0;
  frames++;
  uint32_t now = lv_tick_get();
  if (now - last >= 1000)
  {
    char buf[24];
    lv_snprintf(buf, sizeof(buf), "FPS: %u", (unsigned)frames);
    lv_label_set_text(fpsLabel, buf);
    frames = 0;
    last = now;
  }
}

void MainInterface::screenTouchEvent(lv_event_t *e)
{
  MainInterface *self = static_cast<MainInterface *>(lv_event_get_user_data(e));
  if (!self)
    return;

  lv_point_t p;
  // lv_indev_get_point returns void in LVGL v8; coordinates are valid during press events
  lv_indev_get_point(lv_indev_get_act(), &p);

  // Update marker
  lv_obj_clear_flag(self->touchMarker, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_pos(self->touchMarker, p.x - 5, p.y - 5);

  // Update info label
  lv_event_code_t code = lv_event_get_code(e);
  const char *state = (code == LV_EVENT_RELEASED) ? "released" : "pressed";
  char ibuf[48];
  lv_snprintf(ibuf, sizeof(ibuf), "Touch: %d,%d  state: %s", (int)p.x, (int)p.y, state);
  lv_label_set_text(self->infoLabel, ibuf);

  // Trail
  if (self->trailEnabled && (code == LV_EVENT_PRESSED || code == LV_EVENT_PRESSING))
  {
    // Reuse objects in a ring buffer
    lv_obj_t *dot = self->trail[self->trailIndex];
    if (dot == nullptr)
    {
      dot = lv_obj_create(self->mainScreen);
      lv_obj_remove_style_all(dot);
      lv_obj_set_size(dot, 6, 6);
      lv_obj_set_style_bg_color(dot, lv_color_hex(0xFF7A00), 0);
      lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
      self->trail[self->trailIndex] = dot;
    }
    lv_obj_set_pos(dot, p.x - 3, p.y - 3);
    self->trailIndex = (self->trailIndex + 1) % TRAIL_CAP;
  }
}
