
#include "Ram.h"
#include <cstdint>
#include "spdlog/spdlog.h"

uint8_t Ram::read(const uint16_t address) const {
    if (address >= _size) {
        spdlog::error("Ram::read: address out of bounds");
        return 0;
    }

    return _ram[address];
}

void Ram::write(const uint16_t address, const uint8_t data) {
    if (address >= _size) {
        spdlog::error("Ram::write: address out of bounds");
        return;
    }
    _ram[address] = data;
}
