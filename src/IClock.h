#pragma once

#include <cstdint>

class IClock {
 public:
  virtual ~IClock() = default;

  virtual void update(const uint32_t cycles) = 0;
};
