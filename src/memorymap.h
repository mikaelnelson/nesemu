#pragma once

#include <memory>
#include <set>

#include "memoryinterface.h"

class MemoryMap : public MemoryInterface {
 public:
  bool register_device(const std::shared_ptr<MemoryInterface> &device,
                       const uint16_t address, const uint16_t size);

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  struct MapEntry {
    uint16_t address;
    uint16_t size;
    std::shared_ptr<MemoryInterface> device;

    bool operator<(const MapEntry &other) const {
      if (address <= other.address) {
        return (address + size <= other.address);
      } else if (address >= other.address) {
        return (other.address + other.size <= address);
      } else {
        return false;
      }
    }
  };

  std::set<MapEntry> _address_map;
};
