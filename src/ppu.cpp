#include "ppu.h"

#include <cstdint>

#include "spdlog/spdlog.h"

uint8_t Ppu::read(const uint16_t address) const {
  if (address >= _size) {
    spdlog::error("Ppu::read: address out of bounds");
    return 0;
  }

  spdlog::info("Ppu::read: 0x{0:X}", address);

  return 0;
}

void Ppu::write(const uint16_t address, const uint8_t data) {
  if (address >= _size) {
    spdlog::error("Ppu::write: address out of bounds");
    return;
  }
  spdlog::info("Ppu::write: 0x{0:X} to 0x{1:X}", data, address);
}
