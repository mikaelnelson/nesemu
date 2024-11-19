#pragma once
#include <cstdint>

#include "ISubject.h"

class Clock : public ISubject<uint32_t> {
 public:
  explicit Clock(const uint32_t frequency, const uint32_t cycles_per_update)
      : _frequency(frequency),
        _cycles_per_update(cycles_per_update),
        _time_remainder(0),
        _cycles_remainder(0) {}

  void run(float elapsed_time_sec);

 private:
  const uint32_t _frequency;          // Clock frequency in Hz
  const uint32_t _cycles_per_update;  // Cycles per update call
  float _time_remainder;
  uint32_t _cycles_remainder;
};
