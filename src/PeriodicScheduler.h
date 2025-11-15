#pragma once

#include <functional>
#include <vector>
#include <stdint.h>

class PeriodicScheduler
{
public:
  using Task = std::function<void()>;

  PeriodicScheduler() = default;

  // Add a repeating task; returns the index that can be used to remove the task
  int addTask(Task cb, uint32_t intervalMs);
  void removeTask(int id);

  // Call from loop() to execute pending tasks
  void update(uint32_t now = 0);

private:
  struct Entry
  {
    Task cb;
    uint32_t interval;
    uint32_t lastRun;
    bool active;
  };
  std::vector<Entry> tasks;
};
