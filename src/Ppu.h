#pragma once
#include <array>

#include "IMemory.h"
#include "ISubject.h"

static constexpr uint16_t PPU_V_RES = 240;
static constexpr uint16_t PPU_H_RES = 256;
using PpuFrame = std::array<uint8_t, PPU_V_RES * PPU_H_RES>;

class Ppu : public ISubject<PpuFrame>, public IMemory {
 public:
  Ppu() : _size(Registers::COUNT) {};

  uint16_t size() const { return _size; }
  const uint16_t step();

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  struct Registers {
    static constexpr uint8_t COUNT = 8;

    enum Address {
      CONTROLLER = 0,
      MASK = 1,
      STATUS = 2,
      OAM_ADDRESS = 3,
      OAM_DATA = 4,
      SCROLL = 5,
      ADDRESS = 6,
      DATA = 7,
    };

    uint8_t controller = 0;
    uint8_t mask = 0;
    union {
      uint8_t status = 0;
      struct {
        uint8_t : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zero_bit : 1;
        uint8_t vertical_blank : 1;
      } status_bits;
    };
    uint8_t oam_address = 0;
    uint8_t oam_data = 0;
    uint8_t scroll = 0;
    uint8_t address = 0;
    uint8_t data = 0;
  };

  const uint16_t _size;
  uint16_t _cycle = 0;
  uint16_t _scanline = 0;
  mutable Registers _registers;
};
