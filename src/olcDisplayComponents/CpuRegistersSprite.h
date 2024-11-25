#pragma once

#include <olcPixelGameEngine.h>

#include <tabulate/tabulate.hpp>

#include "Cpu.h"
#include "ISpriteDraw.h"

class CpuRegistersSprite : public ISpriteDraw, public olc::Sprite {
 public:
  CpuRegistersSprite(const std::shared_ptr<Cpu>& cpu, const int32_t width,
                     const int32_t height)
      : olc::Sprite(width, height), _cpu(cpu) {}

  olc::Sprite* draw(olc::PixelGameEngine* pge) override;

 private:
  const std::shared_ptr<Cpu> _cpu;
  Cpu::Registers _prv_cpu_registers;

  static tabulate::Table generate_table(const Cpu::Registers& cpu_registers);
};
