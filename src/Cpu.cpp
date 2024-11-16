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
  update_observers();
  return static_cast<uint16_t>(cycles_ran);
}

CpuStatus Cpu::get_cpu_status() {
  return CpuStatus{
      .A = _cpu.GetA(),
      .X = _cpu.GetX(),
      .Y = _cpu.GetY(),
      .SP = _cpu.GetS(),
      .PC = _cpu.GetPC(),
      .P = _cpu.GetP(),
  };
}

void Cpu::update_observers() {
  if (observers_count() > 0) {
    notify_observers(get_cpu_status());
  }
}
