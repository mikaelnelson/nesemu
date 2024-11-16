#pragma once

#include "IMemory.h"

class Ppu : public IMemory {
public:
    explicit Ppu(const uint16_t size) : _size(size) {};

    uint16_t size() const { return _size; }

    // Implements MemoryInterface
    uint8_t read(const uint16_t address) const override;
    void write(const uint16_t address, const uint8_t data) override;

private:
    uint16_t _size = 0;
};
