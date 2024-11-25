#pragma once

#include <olcPixelGameEngine.h>

#include <tabulate/tabulate.hpp>

#include "ISpriteDraw.h"
#include "Ppu.h"

class PpuRegistersSprite : public ISpriteDraw, public olc::Sprite {
 public:
  PpuRegistersSprite(const std::shared_ptr<Ppu>& ppu, const int32_t width,
                     const int32_t height)
      : olc::Sprite(width, height), _ppu(ppu) {}

  olc::Sprite* draw(olc::PixelGameEngine* pge) override;

 private:
  const std::shared_ptr<Ppu> _ppu;
  Ppu::Registers _prv_ppu_registers;
  Ppu::InternalRegisters _prv_int_ppu_registers;

  static tabulate::Table generate_registers_table(Ppu::Registers& registers);
  static tabulate::Table generate_internal_registers_table(
      Ppu::InternalRegisters& registers);
};
