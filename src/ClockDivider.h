#pragma once

#include <cstdint>

#include "IObserver.h"
#include "ISubject.h"
#include "spdlog/spdlog.h"

class ClockDivider : public ISubject<uint32_t>, public IObserver<uint32_t> {
 public:
  explicit ClockDivider(const uint32_t divisor)
      : _divisor(divisor), _remainder(0) {}

  void update(const uint32_t& cycles) override;

 private:
  const uint32_t _divisor;  // Clock divisor
  uint32_t _remainder;
};
