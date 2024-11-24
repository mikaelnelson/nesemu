#pragma once
#include <array>

#include "IMemory.h"
#include "ISubject.h"

static constexpr uint16_t PPU_V_RES = 240;
static constexpr uint16_t PPU_H_RES = 256;
using PpuFrame = std::array<uint8_t, PPU_V_RES * PPU_H_RES>;

class Ppu : public ISubject<PpuFrame>, public IMemory {
 public:
  explicit Ppu(const std::shared_ptr<IMemory> &ppu_map)
      : _ppu_map(ppu_map),
        _size(Registers::COUNT),
        _cycle(0),
        _scanline(SCANLINE_PRE_RENDER) {};

  uint16_t size() const { return _size; }
  const int clock_update(int cycles);

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  static constexpr int CYCLES_PER_SCANLINE = 341, SCANLINES_PER_FRAME = 262;

  static constexpr int SCANLINE_VISIBLE_START = 0, SCANLINE_VISIBLE_END = 239,
                       SCANLINE_POST_RENDER = 240, SCANLINE_VBLANK_START = 241,
                       SCANLINE_VBLANK_END = 260, SCANLINE_PRE_RENDER = 261;

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
    uint16_t address = 0;
    uint8_t data = 0;
  };

  struct InternalRegisters {
    uint8_t w : 1 = 0;  // Write Latch, 0 = High Byte, 1 = Low Byte
  };

  std::shared_ptr<IMemory> _ppu_map;
  const uint16_t _size;
  int _cycle;
  int _scanline;
  mutable Registers _registers;
  mutable InternalRegisters _int_registers;

  void tick();
  void do_vblank(const int scanline, const int cycle);
  void do_pre_render(const int cycle);
};
