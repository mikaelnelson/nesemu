
#include <spdlog/spdlog.h>

#include <filesystem>

#define OLC_PGE_APPLICATION
#include "Cartridge.h"
#include "CartridgeLoader.h"
#include "Cpu.h"
#include "CpuStatusSprite.h"
#include "MemoryMap.h"
#include "Ppu.h"
#include "Ram.h"
#include "olcPixelGameEngine.h"

class NESEmu : public olc::PixelGameEngine {
 public:
  explicit NESEmu(const std::filesystem::path &rom_filepath)
      : _rom_filepath(rom_filepath),
        _memory_map(std::make_shared<MemoryMap>()),
        _cpu(std::make_shared<Cpu>(_memory_map)),
        _cpu_status_sprite(_cpu, 150, 300) {
    sAppName = "NESEmu";
  }

  bool OnUserCreate() override {
    auto internal_ram = std::make_shared<Ram>(0x0800);

    // Register Internal Ram
    _memory_map->register_device(internal_ram, 0x0000, internal_ram->size());

    // Register Internal Ram Mirrors
    _memory_map->register_device(internal_ram, 0x0800, internal_ram->size());
    _memory_map->register_device(internal_ram, 0x1000, internal_ram->size());
    _memory_map->register_device(internal_ram, 0x1800, internal_ram->size());

    // Register DummyPpu
    auto ppu = std::make_shared<Ppu>(8);
    _memory_map->register_device(ppu, 0x2000, ppu->size());

    // Load Cartridge
    auto cartridge_loader = std::make_shared<CartridgeLoader>();

    if (auto cartridge = cartridge_loader->load(_rom_filepath)) {
      (*cartridge)->load(*_memory_map);
    } else {
      spdlog::error("Failed to load cartridge {}", _rom_filepath.string());
      return false;
    }

    _cpu->reset();
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    uint16_t cycles = _cpu->step();
    spdlog::info("Cycles: {}", cycles);

    Clear(olc::BLACK);

    DrawSprite(15, 20, _cpu_status_sprite.draw(this));
    return true;
  }

 private:
  const std::filesystem::path _rom_filepath;
  std::shared_ptr<MemoryMap> _memory_map;
  std::shared_ptr<Cpu> _cpu;
  CpuStatusSprite _cpu_status_sprite;
};

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    spdlog::error("Usage: {} <cartridge file>", argv[0]);
    return 1;
  }

  NESEmu emu(argv[1]);
  if (emu.Construct(640, 480, 1, 1)) emu.Start();

  return 0;
}
