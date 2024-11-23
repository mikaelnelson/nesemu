#include "Clock.h"

void Clock::run(float elapsed_time_sec) {
  float cycles = (elapsed_time_sec * _frequency) + _time_remainder;
  float full_cycles = 0;
  _time_remainder = std::modf(cycles, &full_cycles);

  uint32_t full_cycles_u32 =
      static_cast<uint32_t>(full_cycles) + _cycles_remainder;

  for (; full_cycles_u32 > 0; full_cycles_u32 -= _cycles_per_update) {
    notify_observers(_cycles_per_update);
  }

  _cycles_remainder = full_cycles_u32;
}