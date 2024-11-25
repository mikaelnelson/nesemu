#pragma once
#include <olcPixelGameEngine.h>

#include <memory>

#include "IObserver.h"
#include "ISpriteDraw.h"
#include "Ppu.h"
#include "PpuFrame.h"

class PpuDisplaySprite : public ISpriteDraw,
                         public olc::Sprite,
                         public IObserver<PpuFrame> {
 public:
  PpuDisplaySprite(const std::shared_ptr<Ppu>& ppu)
      : olc::Sprite(PpuFrame::WIDTH, PpuFrame::HEIGHT), _ppu(ppu) {}

  olc::Sprite* draw(olc::PixelGameEngine* pge) override;

  void update(const PpuFrame& frame) override;

 private:
  std::shared_ptr<Ppu> _ppu;
};
