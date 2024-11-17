#pragma once
#include <olcPixelGameEngine.h>

#include <memory>

#include "IObserver.h"
#include "ISpriteDraw.h"
#include "Ppu.h"

class PpuDisplaySprite : public ISpriteDraw,
                         public olc::Sprite,
                         public IObserver<PpuFrame> {
 public:
  PpuDisplaySprite(const std::shared_ptr<Ppu>& ppu)
      : olc::Sprite(PPU_H_RES, PPU_V_RES), _ppu(ppu) {}

  olc::Sprite* draw(olc::PixelGameEngine* pge) override;

  void update(const PpuFrame& frame) override;

 private:
  std::shared_ptr<Ppu> _ppu;
  PpuFrame _frame;
};
