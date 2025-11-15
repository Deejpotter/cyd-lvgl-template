#include "PeriodicScheduler.h"
#include <Arduino.h>

int PeriodicScheduler::addTask(Task cb, uint32_t intervalMs)
{
  Entry e;
  e.cb = cb;
  e.interval = intervalMs;
  e.lastRun = millis();
  e.active = true;
  tasks.push_back(e);
  return (int)tasks.size() - 1;
}

void PeriodicScheduler::removeTask(int id)
{
  if (id < 0 || id >= (int)tasks.size())
    return;
  tasks[id].active = false;
}

void PeriodicScheduler::update(uint32_t now)
{
  if (now == 0)
    now = millis();
  for (auto &e : tasks)
  {
    if (!e.active)
      continue;
    if ((uint32_t)(now - e.lastRun) >= e.interval)
    {
      e.lastRun = now;
      if (e.cb)
        e.cb();
    }
  }
}
