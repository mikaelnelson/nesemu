#include <spdlog/spdlog.h>

#include "cartridge.h"
#include "cartridgeloader.h"
#include "cpu.h"
#include "memorymap.h"
#include "ram.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    spdlog::error("Usage: {} <cartridge file>", argv[0]);
    return 1;
  }

  auto memory_map = std::make_shared<MemoryMap>();
  auto internal_ram = std::make_shared<Ram>(0x0800);

  // Register Internal Ram
  memory_map->register_device(internal_ram, 0x0000, internal_ram->size());

  // Register Internal Ram Mirrors
  memory_map->register_device(internal_ram, 0x0800, internal_ram->size());
  memory_map->register_device(internal_ram, 0x1000, internal_ram->size());
  memory_map->register_device(internal_ram, 0x1800, internal_ram->size());

  auto cartridge_loader = std::make_shared<CartridgeLoader>();

  if (auto cartridge = cartridge_loader->load(argv[1])) {
    (*cartridge)->load(*memory_map);
  } else {
    spdlog::error("Failed to load cartridge {}", argv[1]);
    return 1;
  }

  auto cpu = std::make_shared<Cpu>(memory_map);
  cpu->reset();

  while (true) {
    uint16_t cycles = cpu->step();
    spdlog::info("Cycles: {}", cycles);
  }

  return 0;
}
