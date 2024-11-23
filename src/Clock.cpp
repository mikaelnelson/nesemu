#include "Clock.h"

#include <cmath>

void Clock::run(float elapsed_time_sec) {
  float cycles = (elapsed_time_sec * _frequency) + _time_remainder;
  float full_cycles = 0;
  _time_remainder = std::modf(cycles, &full_cycles);

  _clock_dst->update(static_cast<uint32_t>(full_cycles));
}