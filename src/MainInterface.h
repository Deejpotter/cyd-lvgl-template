// TestInterface.h
#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <lvgl.h>
#include "GRBLController.h"

class MainInterface
{
private:
  GRBLController &grbl;

  lv_obj_t *mainScreen;
  lv_obj_t *statusLabel;
  lv_obj_t *xPlusBtn;
  lv_obj_t *xMinusBtn;
  lv_obj_t *yPlusBtn;
  lv_obj_t *yMinusBtn;
  lv_obj_t *homeBtn;
  lv_obj_t *distanceSlider;
  lv_obj_t *speedSlider;
  lv_obj_t *distanceLabel;
  lv_obj_t *speedLabel;

  static void jogBtn_event_cb(lv_event_t *e);
  static void homeBtn_event_cb(lv_event_t *e);
  static void slider_event_cb(lv_event_t *e);

  float getCurrentDistance();
  int getCurrentSpeed();
  void updateDistanceLabel();
  void updateSpeedLabel();
  // Creates the jog button with the given label text and user data.
  lv_obj_t *createJogButton(lv_obj_t *parent, const char *labelText, const char *userData);

public:
  MainInterface(GRBLController &grblController) : grbl(grblController) {}

  void init();
  void update();
};

#endif
