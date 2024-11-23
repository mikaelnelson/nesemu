#pragma once
#include "IClock.h"
class NesClockTree : public IClock {
 public:
  NesClockTree(IClock* ppu, IClock* cpu)
      : _ppu(ppu), _cpu(cpu), _cycles_remainder(0) {}

  // Implements IClock
  void update(const uint32_t cycles) override {
    uint32_t total_cycles = cycles + _cycles_remainder;
    for (; total_cycles > 0; total_cycles -= PPU_CYCLES_PER_CPU_CYCLE) {
      _ppu->update(PPU_CYCLES_PER_CPU_CYCLE);
      _cpu->update(1);
    }

    _cycles_remainder = total_cycles;
  }

 private:
  static constexpr int PPU_CYCLES_PER_CPU_CYCLE = 3;

  IClock* _ppu;  // Not Owned
  IClock* _cpu;  // Not Owned
  uint32_t _cycles_remainder;
};
