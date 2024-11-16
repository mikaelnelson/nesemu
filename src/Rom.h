#pragma once

#include <memory>
#include <vector>

#include "IMemory.h"

class Rom : public IMemory {
 public:
  explicit Rom(const std::vector<uint8_t> &rom) : _rom(rom) {};

  uint16_t size() const { return _rom.size(); }

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  std::vector<uint8_t> _rom;
};
