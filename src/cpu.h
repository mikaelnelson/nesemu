#pragma once

#include <mos6502.h>

#include <memory>

#include "memoryinterface.h"
#include "spdlog/spdlog.h"

class Cpu {
 public:
  Cpu() = delete;
  explicit Cpu(std::shared_ptr<MemoryInterface> memory_map);

  void reset();
  const uint16_t step();

 private:
  class MemoryMapSingleton {
   protected:
    explicit MemoryMapSingleton(std::shared_ptr<MemoryInterface> memory_map)
        : _memory_map(std::move(memory_map)) {};

    static MemoryMapSingleton *instance;
    std::shared_ptr<MemoryInterface> _memory_map = nullptr;

   public:
    MemoryMapSingleton() = delete;
    MemoryMapSingleton(MemoryMapSingleton &other) = delete;

    void operator=(const MemoryMapSingleton &) = delete;

    static MemoryMapSingleton *get_instance(
        std::shared_ptr<MemoryInterface> memory_map) {
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

  std::shared_ptr<MemoryInterface> _memory_map;
  mos6502 _cpu;
};