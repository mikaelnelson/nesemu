#pragma once

#include <mos6502.h>

#include <memory>

#include "IMemory.h"
#include "ISubject.h"
#include "spdlog/spdlog.h"

struct CpuStatus {
  uint8_t A;    // Accumulator
  uint8_t X;    // Index register X
  uint8_t Y;    // Index register Y
  uint8_t SP;   // Stack pointer
  uint16_t PC;  // Program counter
  uint8_t P;    // Processor status
};

class Cpu : public ISubject<CpuStatus> {
 public:
  Cpu() = delete;
  explicit Cpu(std::shared_ptr<IMemory> memory_map);

  void reset();
  const uint16_t step();

 private:
  class MemoryMapSingleton {
   protected:
    explicit MemoryMapSingleton(std::shared_ptr<IMemory> memory_map)
        : _memory_map(std::move(memory_map)) {};

    static MemoryMapSingleton *instance;
    std::shared_ptr<IMemory> _memory_map = nullptr;

   public:
    MemoryMapSingleton() = delete;
    MemoryMapSingleton(MemoryMapSingleton &other) = delete;

    void operator=(const MemoryMapSingleton &) = delete;

    static MemoryMapSingleton *get_instance(
        std::shared_ptr<IMemory> memory_map) {
      if (instance == nullptr) {
        instance = new MemoryMapSingleton(std::move(memory_map));
      }
      return instance;
    }

    static uint8_t memory_read(uint16_t address) {
      return instance->_memory_map->read(address);
    }
    static void memory_write(uint16_t address, uint8_t value) {
      instance->_memory_map->write(address, value);
    }
  };

  std::shared_ptr<IMemory> _memory_map;
  mos6502 _cpu;

  CpuStatus get_cpu_status();
  void update_observers();
};
