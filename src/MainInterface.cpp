#include "MainInterface.h"

void MainInterface::init()
{
  // Create main screen
  mainScreen = lv_obj_create(NULL);
  lv_scr_load(mainScreen);

  // Set screen to use flex layout
  lv_obj_set_layout(mainScreen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(mainScreen, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(mainScreen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_row(mainScreen, 10, 0); // Add some spacing between flex items

  // Create top status panel
  lv_obj_t *statusPanel = lv_obj_create(mainScreen);
  lv_obj_set_size(statusPanel, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_style_pad_all(statusPanel, 10, 0);

  statusLabel = lv_label_create(statusPanel);
  lv_label_set_text(statusLabel, "GRBL Status: Unknown");
  lv_obj_center(statusLabel);

  // Create settings panel (for sliders)
  lv_obj_t *settingsPanel = lv_obj_create(mainScreen);
  lv_obj_set_size(settingsPanel, LV_PCT(90), LV_SIZE_CONTENT);
  lv_obj_set_layout(settingsPanel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(settingsPanel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_all(settingsPanel, 10, 0);
  lv_obj_set_style_pad_row(settingsPanel, 15, 0);

  // Distance control
  lv_obj_t *distanceContainer = lv_obj_create(settingsPanel);
  lv_obj_set_size(distanceContainer, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_layout(distanceContainer, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(distanceContainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(distanceContainer, 5, 0);

  distanceLabel = lv_label_create(distanceContainer);
  lv_label_set_text(distanceLabel, "Distance: 1.0 mm");

  distanceSlider = lv_slider_create(distanceContainer);
  lv_obj_set_width(distanceSlider, LV_PCT(100));
  lv_slider_set_range(distanceSlider, 0, 4);
  lv_slider_set_value(distanceSlider, 1, LV_ANIM_OFF);
  lv_obj_add_event_cb(distanceSlider, slider_event_cb, LV_EVENT_VALUE_CHANGED, this);

  // Speed control
  lv_obj_t *speedContainer = lv_obj_create(settingsPanel);
  lv_obj_set_size(speedContainer, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_layout(speedContainer, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(speedContainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(speedContainer, 5, 0);

  speedLabel = lv_label_create(speedContainer);
  lv_label_set_text(speedLabel, "Speed: 500 mm/min");

  speedSlider = lv_slider_create(speedContainer);
  lv_obj_set_width(speedSlider, LV_PCT(100));
  lv_slider_set_range(speedSlider, 0, 4);
  lv_slider_set_value(speedSlider, 1, LV_ANIM_OFF);
  lv_obj_add_event_cb(speedSlider, slider_event_cb, LV_EVENT_VALUE_CHANGED, this);

  // Create jog control panel
  lv_obj_t *jogPanel = lv_obj_create(mainScreen);
  lv_obj_set_size(jogPanel, LV_PCT(90), LV_SIZE_CONTENT);
  lv_obj_set_layout(jogPanel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(jogPanel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_all(jogPanel, 10, 0);
  lv_obj_set_style_pad_row(jogPanel, 10, 0);

  // Top row (Y+)
  lv_obj_t *topRow = lv_obj_create(jogPanel);
  lv_obj_set_size(topRow, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_layout(topRow, LV_LAYOUT_FLEX);
  lv_obj_set_flex_align(topRow, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_bottom(topRow, 10, 0);

  yPlusBtn = createJogButton(topRow, "Y+", "Y+");

  // Middle row (X-, Home, X+)
  lv_obj_t *middleRow = lv_obj_create(jogPanel);
  lv_obj_set_size(middleRow, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_layout(middleRow, LV_LAYOUT_FLEX);
  lv_obj_set_flex_align(middleRow, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  xMinusBtn = createJogButton(middleRow, "X-", "X-");
  homeBtn = createJogButton(middleRow, "Home", "HOME");
  xPlusBtn = createJogButton(middleRow, "X+", "X+");

  // Bottom row (Y-)
  lv_obj_t *bottomRow = lv_obj_create(jogPanel);
  lv_obj_set_size(bottomRow, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_layout(bottomRow, LV_LAYOUT_FLEX);
  lv_obj_set_flex_align(bottomRow, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  yMinusBtn = createJogButton(bottomRow, "Y-", "Y-");

  updateDistanceLabel();
  updateSpeedLabel();
}

lv_obj_t *MainInterface::createJogButton(lv_obj_t *parent, const char *labelText, const char *userData)
{
  lv_obj_t *btn = lv_btn_create(parent);
  lv_obj_set_size(btn, 70, 70);
  lv_obj_add_event_cb(btn, jogBtn_event_cb, LV_EVENT_CLICKED, this);
  lv_obj_set_user_data(btn, (void *)userData);

  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, labelText);
  lv_obj_center(label);

  return btn;
}

float MainInterface::getCurrentDistance()
{
  int32_t value = lv_slider_get_value(distanceSlider);
  switch (value)
  {
  case 0:
    return 0.1f;
  case 1:
    return 1.0f;
  case 2:
    return 5.0f;
  case 3:
    return 10.0f;
  case 4:
    return 50.0f;
  default:
    return 1.0f;
  }
}

int MainInterface::getCurrentSpeed()
{
  int32_t value = lv_slider_get_value(speedSlider);
  switch (value)
  {
  case 0:
    return 100;
  case 1:
    return 500;
  case 2:
    return 1000;
  case 3:
    return 2000;
  case 4:
    return 5000;
  default:
    return 500;
  }
}

void MainInterface::updateDistanceLabel()
{
  char buf[32];
  snprintf(buf, sizeof(buf), "Distance: %.1f mm", getCurrentDistance());
  lv_label_set_text(distanceLabel, buf);
}

void MainInterface::updateSpeedLabel()
{
  char buf[32];
  snprintf(buf, sizeof(buf), "Speed: %d mm/min", getCurrentSpeed());
  lv_label_set_text(speedLabel, buf);
}

void MainInterface::slider_event_cb(lv_event_t *e)
{
  MainInterface *self = (MainInterface *)lv_event_get_user_data(e);
  lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);

  if (slider == self->distanceSlider)
  {
    self->updateDistanceLabel();
  }
  else if (slider == self->speedSlider)
  {
    self->updateSpeedLabel();
  }
}

void MainInterface::jogBtn_event_cb(lv_event_t *e)
{
  MainInterface *self = (MainInterface *)lv_event_get_user_data(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  const char *dir = (const char *)lv_obj_get_user_data(btn);

  float distance = self->getCurrentDistance();
  int speed = self->getCurrentSpeed();

  char cmd[50];
  if (strcmp(dir, "X+") == 0)
  {
    snprintf(cmd, sizeof(cmd), "G91 G0 X%.1f F%d", distance, speed);
  }
  else if (strcmp(dir, "X-") == 0)
  {
    snprintf(cmd, sizeof(cmd), "G91 G0 X-%.1f F%d", distance, speed);
  }
  else if (strcmp(dir, "Y+") == 0)
  {
    snprintf(cmd, sizeof(cmd), "G91 G0 Y%.1f F%d", distance, speed);
  }
  else if (strcmp(dir, "Y-") == 0)
  {
    snprintf(cmd, sizeof(cmd), "G91 G0 Y-%.1f F%d", distance, speed);
  }

  self->grbl.sendCommand(cmd);
  String response = self->grbl.waitForResponse();
  lv_label_set_text(self->statusLabel, response.c_str());
}

void MainInterface::homeBtn_event_cb(lv_event_t *e)
{
  MainInterface *self = (MainInterface *)lv_event_get_user_data(e);
  self->grbl.sendCommand("$H"); // GRBL home command
  String response = self->grbl.waitForResponse();
  lv_label_set_text(self->statusLabel, response.c_str());
}

void MainInterface::update()
{
  // Add any periodic updates here if needed
}
