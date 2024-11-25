#pragma once

#include <mos6502.h>

#include <memory>

#include "IMemory.h"
#include "ISubject.h"
#include "spdlog/spdlog.h"

class Cpu {
 public:
  struct Registers {
    Registers() : a(0), x(0), y(0), sp(0), pc(0), p(0) {}
    Registers(uint8_t a, uint8_t x, uint8_t y, uint8_t sp, uint16_t pc,
              uint8_t p)
        : a(a), x(x), y(y), sp(sp), pc(pc), p(p) {}
    uint8_t a;    // Accumulator
    uint8_t x;    // Index register x
    uint8_t y;    // Index register y
    uint8_t sp;   // Stack pointer
    uint16_t pc;  // Program counter
    union {
      uint8_t p;  // Processor status
      struct {
        uint8_t carry : 1;              // Carry flag
        uint8_t zero : 1;               // Zero flag
        uint8_t interrupt_disable : 1;  // Interrupt disable flag
        uint8_t decimal : 1;            // Decimal mode flag
        uint8_t : 1;                    // (No CPU effect; see: the B flag)
        uint8_t : 1;                    // (No CPU effect; always pushed as 1)
        uint8_t overflow : 1;           // Overflow flag
        uint8_t negative : 1;           // Negative flag
      } p_bits;
    };
    bool operator==(const Registers &cpu_status) const {
      return 0 == memcmp(this, &cpu_status, sizeof(Registers));
    };
  };

  Cpu() = delete;
  explicit Cpu(std::shared_ptr<IMemory> memory_map);

  void reset();
  const int clock_update(int cycles);
  Registers get_registers();

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
};
