#pragma once

#include "Cpu.h"
#include "olcPixelGameEngine.h"

class CpuStatusSprite : public olc::Sprite {
 public:
  CpuStatusSprite(const std::shared_ptr<Cpu>& cpu, const int32_t width,
                  const int32_t height)
      : _cpu(cpu), olc::Sprite(width, height) {}

  olc::Sprite* draw(olc::PixelGameEngine* pge);

 private:
  const std::shared_ptr<Cpu> _cpu;
};
