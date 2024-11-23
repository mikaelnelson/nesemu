#pragma once

#include <mos6502.h>

#include <memory>

#include "IClock.h"
#include "IMemory.h"
#include "spdlog/spdlog.h"

struct CpuStatus {
  CpuStatus() : A(0), X(0), Y(0), SP(0), PC(0), P(0) {}
  CpuStatus(uint8_t a, uint8_t x, uint8_t y, uint8_t sp, uint16_t pc, uint8_t p)
      : A(a), X(x), Y(y), SP(sp), PC(pc), P(p) {}
  uint8_t A;    // Accumulator
  uint8_t X;    // Index register X
  uint8_t Y;    // Index register Y
  uint8_t SP;   // Stack pointer
  uint16_t PC;  // Program counter
  union {
    uint8_t P;  // Processor status
    struct {
      uint8_t carry : 1;              // Carry flag
      uint8_t zero : 1;               // Zero flag
      uint8_t interrupt_disable : 1;  // Interrupt disable flag
      uint8_t decimal : 1;            // Decimal mode flag
      uint8_t : 1;                    // (No CPU effect; see: the B flag)
      uint8_t : 1;                    // (No CPU effect; always pushed as 1)
      uint8_t overflow : 1;           // Overflow flag
      uint8_t negative : 1;           // Negative flag
    } PBits;
  };
  bool operator==(const CpuStatus &cpu_status) const {
    return 0 == memcmp(this, &cpu_status, sizeof(CpuStatus));
  };
};

class Cpu {
 public:
  Cpu() = delete;
  explicit Cpu(std::shared_ptr<IMemory> memory_map);

  void reset();
  const uint16_t step();
  CpuStatus get_cpu_status();
  IClock *clock_signal() { return &_clock; }

 private:
  class Clock : public IClock {
   public:
    Clock() = default;

    void update(const uint32_t cycles) override {
      spdlog::info("CPU Cycles: {}", cycles);
    }
  };

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
  Clock _clock;
  mos6502 _cpu;
};
