#include "memorymap.h"

#include <spdlog/spdlog.h>

bool MemoryMap::register_device(const std::shared_ptr<MemoryInterface> &device,
                                const uint16_t address, const uint16_t size) {
  auto [it, result] = _address_map.insert(MapEntry{address, size, device});

  if (!result) {
    spdlog::warn(
        "Device (0x{0:X} - 0x{1:X}) overlaps with existing device (0x{2:X} - "
        "0x{3:X}), cannot register",
        address, address + size - 1, it->address, it->address - it->size - 1);
  }
  return result;
}

uint8_t MemoryMap::read(const uint16_t address) const {
  auto result = _address_map.find(MapEntry{address, 1, nullptr});

  if (result != _address_map.end()) {
    return result->device->read(address - result->address);
  } else {
    spdlog::warn("Read from memory address 0x{0:x} failed, no device found",
                 address);
  }

  return 0;
}

void MemoryMap::write(const uint16_t address, const uint8_t data) {
  auto result = _address_map.find(MapEntry{address, 1, nullptr});

  if (result != _address_map.end()) {
    return result->device->write(address - result->address, data);
  } else {
    spdlog::warn("Write to memory address 0x{0:x} failed, no device found",
                 address);
  }
}