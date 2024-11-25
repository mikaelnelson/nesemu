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
    case CONTROLLER:
    case MASK:
      // Write Only
      return 0;

    case STATUS: {
      const auto status = _registers.status;
      _registers.status_bits.vertical_blank = false;

      _int_registers.w = false;
      return status;
    }

    case OAM_ADDRESS:
      return _registers.oam_address;

    case OAM_DATA:
      return _registers.oam_data;

    case SCROLL:
      // Write Only
      return 0;

    case ADDRESS:
      // Write Only
      return 0;

    case DATA: {
      static uint8_t cached_data = 0;
      _registers.data = cached_data;
      cached_data = _ppu_map->read(_registers.address);

      return _registers.data;
    }

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

  spdlog::info("Ppu::write: 0x{0:X}", address);

  switch (address) {
    case CONTROLLER:
      _registers.controller = data;
      return;

    case MASK:
      _registers.mask = data;
      return;

    case STATUS:
      // Read Only
      return;

    case OAM_ADDRESS:
      _registers.oam_address = data;
      return;

    case OAM_DATA:
      _registers.oam_data = data;
      return;

    case SCROLL:
      _registers.scroll = data;
      return;

    case ADDRESS: {
      if (_int_registers.w) {
        // Write Low Byte
        _registers.address &= 0xFF00;
        _registers.address |= data;
      } else {
        // Write High Byte
        _registers.address &= 0x00FF;
        _registers.address |= (data << 8);
        _int_registers.w = true;
      }
      _registers.address &= 0x3FFF;
      return;
    }

    case DATA:
      _registers.data = data;
      _ppu_map->write(_registers.address, _registers.data);
      _registers.address++;
      _registers.address &= 0x3FFF;
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
    do_pre_render(_cycle);
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

    // Write Noise to Frame
    for (int x = 0; x < PpuFrame::WIDTH; x++) {
      for (int y = 0; y < PpuFrame::HEIGHT; y++) {
        _ppu_frame.set_pixel(x, y, rand() % 256, rand() % 256, rand() % 256);
      }
    }

    notify_observers(_ppu_frame);
  }
}

void Ppu::do_vblank(const int scanline, const int cycle) {
  if (scanline == SCANLINE_VBLANK_START && cycle == 1) {
    spdlog::info("Ppu::do_vblank");
    _registers.status_bits.vertical_blank = 1;

    // @todo: Emit VBlank NMI Signal Here If Enabled
  }
}

void Ppu::do_pre_render(const int cycle) {
  if (cycle == 1) {
    _registers.status_bits.vertical_blank = 0;
  }
}