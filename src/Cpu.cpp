#include "Cpu.h"

Cpu::MemoryMapSingleton *Cpu::MemoryMapSingleton::instance = nullptr;

Cpu::Cpu(std::shared_ptr<IMemory> memory_map)
    : _memory_map(std::move(memory_map)),
      _cpu(Cpu::MemoryMapSingleton::memory_read,
           MemoryMapSingleton::memory_write) {
  // Construct & provide Memory Map to Singleton
  MemoryMapSingleton::get_instance(_memory_map);
}

void Cpu::reset() { _cpu.Reset(); }

const uint16_t Cpu::step() {
  uint64_t cycles_ran = 0;
  _cpu.Run(1, cycles_ran, mos6502::CycleMethod::INST_COUNT);
  return static_cast<uint16_t>(cycles_ran);
}

CpuStatus Cpu::get_cpu_status() {
  return CpuStatus(_cpu.GetA(), _cpu.GetX(), _cpu.GetY(), _cpu.GetS(),
                   _cpu.GetPC(), _cpu.GetP());
}