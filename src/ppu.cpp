#include "ppu.h"

#include <cstdint>

#include "spdlog/spdlog.h"

uint8_t Ppu::read(const uint16_t address) const {
  if (address >= _size) {
    spdlog::error("Ppu::read: address out of bounds");
    return 0;
  }

  switch (address) {
    case Register::CONTROLLER:
    case Register::MASK:
      // Write Only
      return 0;

    case Register::STATUS:
      return _register.status;

    case Register::OAM_ADDRESS:
      return _register.oam_address;

    case Register::OAM_DATA:
      return _register.oam_data;

    case Register::SCROLL:
      // Write Only
      return 0;

    case Register::ADDRESS:
      return _register.address;

    case Register::DATA:
      return _register.data;

    default:
      spdlog::error("Ppu::read: unknown register address: {}", address);
      return 0;
  }
}

void Ppu::write(const uint16_t address, const uint8_t data) {
  if (address >= _size) {
    spdlog::error("Ppu::write: address out of bounds");
    return;
  }
  spdlog::info("Ppu::write: 0x{0:X} to 0x{1:X}", data, address);

  switch (address) {
    case Register::CONTROLLER:
      _register.controller = data;
      break;

    case Register::MASK:
      _register.mask = data;
      break;

    case Register::STATUS:
      // Read Only
      return;

    case Register::OAM_ADDRESS:
      _register.oam_address = data;
      return;

    case Register::OAM_DATA:
      _register.oam_data = data;
      return;

    case Register::SCROLL:
      _register.scroll = data;
      return;

    case Register::ADDRESS:
      _register.address = data;  // Palette
      return;

    case Register::DATA:
      _register.data = data;  // Palette
      return;

    default:
      spdlog::error("Ppu::write: unknown register address: {}", address);
      return;
  }
}
