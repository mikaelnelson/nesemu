#include "MemoryMap.h"

#include <spdlog/spdlog.h>

bool MemoryMap::register_device(const std::shared_ptr<IMemory> &device,
                                const uint16_t address, const uint16_t size) {
  bool result = add_entry(MapEntry{address, size, device});

  if (!result) {
    spdlog::warn(
        "Device (0x{0:X} - 0x{1:X}) overlaps with existing "
        "device, cannot "
        "register",
        address, address + size - 1);
  }
  return result;
}

uint8_t MemoryMap::read(const uint16_t address) const {
  if (auto entry = find_entry(address)) {
    return entry->device->read(address - entry->address);
  } else {
    spdlog::warn("Read from memory address 0x{0:x} failed, no device found",
                 address);
  }

  return 0;
}

void MemoryMap::write(const uint16_t address, const uint8_t data) {
  if (auto entry = find_entry(address)) {
    return entry->device->write(address - entry->address, data);
  } else {
    spdlog::warn("Write to memory address 0x{0:x} failed, no device found",
                 address);
  }
}

std::optional<MemoryMap::MapEntry> MemoryMap::find_entry(
    const uint16_t address) const {
  for (const auto &entry : _address_map) {
    if (address >= entry.address && address < entry.address + entry.size) {
      return entry;
    }
  }
  return std::nullopt;
}

bool MemoryMap::add_entry(const MapEntry &new_entry) {
  uint16_t new_entry_start = new_entry.address;
  uint16_t new_entry_end = new_entry.address + new_entry.size - 1;

  for (const auto &map_entry : _address_map) {
    uint16_t map_entry_start = map_entry.address;
    uint16_t map_entry_end = map_entry.address + map_entry.size - 1;

    // Does new_entry start exist within map_entry?
    if (new_entry_start >= map_entry_start && new_entry_end <= map_entry_end) {
      return false;
    }
    // Does the new_entry end existing within the map_entry
    else if (new_entry_end >= map_entry_start &&
             new_entry_end <= map_entry_end) {
      return false;
    }
  }

  auto [it, result] = _address_map.insert(new_entry);

  return result;
}
