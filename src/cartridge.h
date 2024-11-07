#pragma once

#include <memory>
#include <vector>

#include "memorymap.h"

class Cartridge {
 public:
  Cartridge() = delete;
  explicit Cartridge(const std::vector<uint8_t> &data) : _data(data) {};
  virtual ~Cartridge() = default;

  virtual bool load(MemoryMap &memory_map, MemoryMap &ppu_map) = 0;

 protected:
  std::vector<uint8_t> _data;
};
