
#include "rom.h"

#include <cstdint>

#include "spdlog/spdlog.h"

uint8_t Rom::read(const uint16_t address) const {
  if (address >= size()) {
    spdlog::error("Rom::read: address out of bounds");
    return 0;
  }

  return _rom[address];
}

void Rom::write(const uint16_t address, const uint8_t data) {
  spdlog::error("Rom::write: cannot write to read only memory");
}
