#pragma once
#include <array>

#include "IMemory.h"
#include "ISubject.h"
#include "MemoryMap.h"
#include "PpuFrame.h"
#include "Ram.h"

class Ppu : public ISubject<PpuFrame>, public IMemory {
 public:
  struct Registers {
    static constexpr uint8_t COUNT = 8;

    union {
      uint8_t controller = 0;
      struct {
        uint8_t vblank_nmi_enable : 1;
        uint8_t ppu_master_slave_select : 1;
        uint8_t sprite_size : 1;
        uint8_t background_pattern_table_addr : 1;
        uint8_t sprite_pattern_table_addr : 1;
        uint8_t vram_address_increment : 1;
        uint8_t base_nametable_address : 2;
      } controller_bits;
    };
    uint8_t mask = 0;
    union {
      uint8_t status = 0;
      struct {
        uint8_t : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zero_hit : 1;
        uint8_t vertical_blank : 1;
      } status_bits;
    };
    uint8_t oam_address = 0;
    uint8_t oam_data = 0;
    uint8_t scroll = 0;
    uint16_t address = 0;
    uint8_t data = 0;

    bool operator==(const Registers &registers) const {
      return 0 == memcmp(this, &registers, sizeof(Registers));
    };
  };

  struct InternalRegisters {
    uint8_t w : 1 = 0;  // Write Latch, 0 = High Byte, 1 = Low Byte

    bool operator==(const InternalRegisters &registers) const {
      return 0 == memcmp(this, &registers, sizeof(InternalRegisters));
    };
  };

  explicit Ppu(const std::shared_ptr<MemoryMap> &ppu_map)
      : _ppu_map(ppu_map),
        _vram(std::make_shared<Ram>(0x1000)),
        _size(Registers::COUNT),
        _cycle(0),
        _scanline(SCANLINE_PRE_RENDER) {
    _ppu_map->register_device(_vram, 0x2000, _vram->size());

    // @note: Mirror VRAM, omit last 0x100 bytes
    _ppu_map->register_device(_vram, 0x3000, _vram->size() - 0x100);
  };

  uint16_t size() const { return _size; }
  const int clock_update(int cycles);

  Registers get_registers() const { return _registers; }
  InternalRegisters get_internal_registers() const { return _int_registers; }

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  static constexpr int CYCLES_PER_SCANLINE = 341, SCANLINES_PER_FRAME = 262;

  static constexpr int SCANLINE_VISIBLE_START = 0, SCANLINE_VISIBLE_END = 239,
                       SCANLINE_POST_RENDER = 240, SCANLINE_VBLANK_START = 241,
                       SCANLINE_VBLANK_END = 260, SCANLINE_PRE_RENDER = 261;

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

  std::shared_ptr<MemoryMap> _ppu_map;
  std::shared_ptr<Ram> _vram;
  const uint16_t _size;
  int _cycle;
  int _scanline;
  mutable Registers _registers;
  mutable InternalRegisters _int_registers;
  PpuFrame _ppu_frame;

  void tick();
  void do_vblank(const int scanline, const int cycle);
  void do_pre_render(const int cycle);
};
