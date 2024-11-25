
#include <spdlog/spdlog.h>

#include <filesystem>

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

#include "Cartridge.h"
#include "CartridgeLoader.h"
#include "Cpu.h"
#include "CpuRegistersSprite.h"
#include "MemoryMap.h"
#include "Ppu.h"
#include "PpuDisplaySprite.h"
#include "PpuRegistersSprite.h"
#include "Ram.h"

class NESEmu : public olc::PixelGameEngine, public IObserver<PpuFrame> {
 public:
  explicit NESEmu(const std::filesystem::path &rom_filepath)
      : _rom_filepath(rom_filepath),
        _memory_map(std::make_shared<MemoryMap>()),
        _ppu_map(std::make_shared<MemoryMap>()),
        _cpu(std::make_shared<Cpu>(_memory_map)),
        _ppu(std::make_shared<Ppu>(_ppu_map)),
        _cpu_status_sprite(_cpu, 150, 300),
        _ppu_registers_sprite(_ppu, 170, 300),
        _ppu_display_sprite(_ppu) {
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

    // Register Ppu
    _memory_map->register_device(_ppu, 0x2000, _ppu->size());

    // Load Cartridge
    auto cartridge_loader = std::make_shared<CartridgeLoader>();

    if (auto cartridge = CartridgeLoader::load(_rom_filepath)) {
      (*cartridge)->load(*_memory_map, *_ppu_map);
    } else {
      spdlog::error("Failed to load cartridge {}", _rom_filepath.string());
      return false;
    }

    _ppu->add_observer(&_ppu_display_sprite);
    _ppu->add_observer(this);

    _cpu->reset();
    return true;
  }

  bool OnUserUpdate(float elapsed_time) override {
    if (_residual_time >= 0.0f) {
      _residual_time -= elapsed_time;
    } else {
      _residual_time += (1.0f / 60.0f) - elapsed_time;

      do {
        _cpu->clock_update(1);
        _ppu->clock_update(3);
      } while (!_frame_ready);
      _frame_ready = false;
    }

    Clear(olc::BLACK);

    DrawSprite(0, 0, _ppu_display_sprite.draw(this));
    DrawSprite(_ppu_display_sprite.width, 0, _cpu_status_sprite.draw(this));
    DrawSprite(_ppu_display_sprite.width + _cpu_status_sprite.width, 0,
               _ppu_registers_sprite.draw(this));

    return true;
  }

  void update(const PpuFrame &) override { _frame_ready = true; }

 private:
  const std::filesystem::path _rom_filepath;
  std::shared_ptr<MemoryMap> _memory_map;
  std::shared_ptr<MemoryMap> _ppu_map;
  std::shared_ptr<Cpu> _cpu;
  std::shared_ptr<Ppu> _ppu;
  CpuRegistersSprite _cpu_status_sprite;
  PpuRegistersSprite _ppu_registers_sprite;
  PpuDisplaySprite _ppu_display_sprite;
  bool _frame_ready = false;
  float _residual_time = 0.0f;
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
