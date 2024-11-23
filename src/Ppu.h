#pragma once
#include <array>

#include "IClock.h"
#include "IMemory.h"
#include "ISubject.h"
#include "spdlog/spdlog.h"

static constexpr uint16_t PPU_V_RES = 240;
static constexpr uint16_t PPU_H_RES = 256;
using PpuFrame = std::array<uint8_t, PPU_V_RES * PPU_H_RES>;

class Ppu : public ISubject<PpuFrame>, public IMemory {
 public:
  explicit Ppu(const uint16_t size) : _size(size) {};

  uint16_t size() const { return _size; }
  IClock *clock_signal() { return &_clock; }

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  class Clock : public IClock {
   public:
    Clock() = default;

    void update(const uint32_t cycles) override {
      spdlog::info("PPU Cycles: {}", cycles);
    }
  };

  Clock _clock;
  uint16_t _size = 0;
};
