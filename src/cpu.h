#pragma once

#include "memoryinterface.h"

#include <memory>

class Cpu {
 public:
  Cpu() = delete;
  Cpu(std::shared_ptr<MemoryInterface> memory_map);

 private:
  std::shared_ptr<MemoryInterface> _memory_map;
};