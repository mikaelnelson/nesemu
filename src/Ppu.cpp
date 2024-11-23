#include "Ppu.h"

#include <cstdint>

#include "spdlog/spdlog.h"

const int Ppu::clock_update(int cycles) {
  for (int i = 0; i < cycles; i++) {
    tick();
  }
  return cycles;
}

uint8_t Ppu::read(const uint16_t address) const {
  if (address >= _size) {
    spdlog::error("Ppu::read: address out of bounds");
    return 0;
  }

  spdlog::info("Ppu::read: 0x{0:X}", address);

  switch (address) {
    case Registers::CONTROLLER:
    case Registers::MASK:
      // Write Only
      return 0;

    case Registers::STATUS: {
      const auto status = _registers.status;
      _registers.status_bits.vertical_blank = false;
      return status;
    }

    case Registers::OAM_ADDRESS:
      return _registers.oam_address;

    case Registers::OAM_DATA:
      return _registers.oam_data;

    case Registers::SCROLL:
      // Write Only
      return 0;

    case Registers::ADDRESS:
      return _registers.address;

    case Registers::DATA:
      return _registers.data;

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
  switch (address) {
    case Registers::CONTROLLER:
      _registers.controller = data;
      return;

    case Registers::MASK:
      _registers.mask = data;
      return;

    case Registers::STATUS:
      // Read Only
      return;

    case Registers::OAM_ADDRESS:
      _registers.oam_address = data;
      return;

    case Registers::OAM_DATA:
      _registers.oam_data = data;
      return;

    case Registers::SCROLL:
      _registers.scroll = data;
      return;

    case Registers::ADDRESS:
      _registers.address = data;  // Palette
      return;

    case Registers::DATA:
      _registers.data = data;  // Palette
      return;

    default:
      spdlog::error("Ppu::write: unknown register address: {}", address);
      return;
  }
}

void Ppu::tick() {
  if (_scanline >= SCANLINE_VISIBLE_START &&
      _scanline <= SCANLINE_VISIBLE_END) {
    // Visible Area
  } else if (_scanline == SCANLINE_POST_RENDER) {
  } else if (_scanline >= SCANLINE_VBLANK_START &&
             _scanline <= SCANLINE_VBLANK_END) {
    do_vblank(_scanline, _cycle);
  } else if (_scanline == SCANLINE_PRE_RENDER) {
  } else {
    spdlog::error("Ppu::tick: invalid scanline {}", _scanline);
  }

  if (_cycle >= CYCLES_PER_SCANLINE) {
    _cycle = 0;
    _scanline++;
  } else {
    _cycle++;
  }

  if (_scanline >= SCANLINES_PER_FRAME) {
    // End of Frame
    _scanline = 0;
  }
}

void Ppu::do_vblank(const int scanline, const int cycle) {
  if (scanline == SCANLINE_VBLANK_START && cycle == 1) {
    // @todo: Emit VBlank NMI Signal Here
    spdlog::info("Ppu::do_vblank");
    _registers.status_bits.vertical_blank = 1;
  }
}