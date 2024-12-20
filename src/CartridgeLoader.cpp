#include "CartridgeLoader.h"

#include <fstream>
#include <iostream>

#include "Cartridge.h"
#include "Ram.h"
#include "Rom.h"
#include "spdlog/spdlog.h"

std::optional<std::shared_ptr<Cartridge>> CartridgeLoader::load(
    const std::filesystem::path &rom_filepath) {
  std::ifstream file(rom_filepath, std::ios::binary);

  if (!file.is_open()) {
    spdlog::error("CartridgeLoader::load: Failed to open file '{}'",
                  rom_filepath.string());
    return std::nullopt;
  }

  const std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());

  const std::string file_ext = rom_filepath.extension().string();

  if (file_ext == ".nes") {
    return std::make_shared<Cartridge>(buffer);
  }

  return std::nullopt;
}
