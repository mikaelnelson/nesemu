#pragma once

#include <tabulate/tabulate.hpp>

#include "Cpu.h"
#include "olcPixelGameEngine.h"

class CpuStatusSprite : public olc::Sprite {
 public:
  CpuStatusSprite(const std::shared_ptr<Cpu>& cpu, const int32_t width,
                  const int32_t height)
      : olc::Sprite(width, height), _cpu(cpu) {}

  olc::Sprite* draw(olc::PixelGameEngine* pge);

 private:
  const std::shared_ptr<Cpu> _cpu;
  CpuStatus _prv_cpu_status;

  static tabulate::Table generate_table(const CpuStatus& status);
};
