#pragma once
#include <cstdint>

#include "IClock.h"

class Clock {
 public:
  explicit Clock(const uint32_t frequency, IClock* clock_dst)
      : _frequency(frequency), _time_remainder(0), _clock_dst(clock_dst) {}

  void run(float elapsed_time_sec);

 private:
  const uint32_t _frequency;  // Clock frequency in Hz
  float _time_remainder;
  IClock* _clock_dst;
};
