#include "ClockDivider.h"

void ClockDivider::update(const uint32_t& cycles) {
  const uint32_t total_cycles = cycles + _remainder;
  const uint32_t div_cycles = total_cycles / _divisor;

  _remainder = total_cycles % _divisor;

  notify_observers(div_cycles);
}