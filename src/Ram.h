#pragma once
#include <memory>
#include "IMemory.h"

class Ram : public IMemory {
public:
    explicit Ram(const uint16_t size) :
        _size(size), _ram(std::make_unique<uint8_t[]>(size)) {};

    uint16_t size() const { return _size; }

    // Implements MemoryInterface
    uint8_t read(const uint16_t address) const override;
    void write(const uint16_t address, const uint8_t data) override;

private:
    uint16_t _size = 0;
    std::unique_ptr<uint8_t[]> _ram;
};
