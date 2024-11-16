#pragma once

#include <memory>
#include <set>

#include "IMemory.h"
#include "spdlog/spdlog.h"

class MemoryMap : public IMemory {
 public:
  bool register_device(const std::shared_ptr<IMemory> &device,
                       const uint16_t address, const uint16_t size);

  // Implements MemoryInterface
  uint8_t read(const uint16_t address) const override;
  void write(const uint16_t address, const uint8_t data) override;

 private:
  struct MapEntry {
    uint16_t address;
    uint16_t size;
    std::shared_ptr<IMemory> device;

    bool operator<(const MapEntry &lookup) const {
      return lookup.address < address;
    }
  };

  std::set<MapEntry> _address_map;

  std::optional<MapEntry> find_entry(const uint16_t address) const;

  bool add_entry(const MapEntry &new_entry);
};
