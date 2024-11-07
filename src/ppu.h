#pragma once
#include <array>

#include "memorymap.h"
#include "ram.h"

class Ppu : public MemoryInterface {
 public:
  Ppu() = delete;
  explicit Ppu(std::shared_ptr<MemoryMap> ppu_map)
      : _ppu_map(ppu_map),
        _palette(std::make_shared<Ram>(PALETTE_SIZE)),
        _vram(std::make_shared<Ram>(VRAM_SIZE)),
        _size(Register::COUNT) {
    ppu_map->register_device(_palette, 0x3F00, _palette->size());
    ppu_map->register_device(_vram, 0x2000, _vram->size());
  };

  uint16_t size() const { return _size; }

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  static constexpr uint16_t PALETTE_SIZE = 0x100;
  static constexpr uint16_t VRAM_SIZE = 0x800;
  static constexpr uint16_t OAM_RAM_SIZE = 0x100;

  struct Register {
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
    uint8_t status = 0;
    uint8_t oam_address = 0;
    uint8_t oam_data = 0;
    uint8_t scroll = 0;
    uint8_t address = 0;
    uint8_t data = 0;
  };

  std::shared_ptr<MemoryMap> _ppu_map;
  std::shared_ptr<Ram> _palette;
  std::shared_ptr<Ram> _vram;
  std::array<uint8_t, OAM_RAM_SIZE> _oam_ram;
  uint16_t _size = 0;
  Register _register;
};
