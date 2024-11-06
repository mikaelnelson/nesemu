#pragma once
#include <filesystem>
#include <optional>

#include "cartridge.h"

class CartridgeLoader {
public:
    static std::optional<std::shared_ptr<Cartridge>>
    load(const std::filesystem::path &rom_filepath);
};
