#pragma once

#include <vector>
#include "MemoryMap.h"

class Cartridge {
public:
    Cartridge() = delete;
    explicit Cartridge(const std::vector<uint8_t> &data) : _data(data) {};
    virtual ~Cartridge() = default;

    virtual bool load(MemoryMap &map) = 0;

protected:
    std::vector<uint8_t> _data;
};
