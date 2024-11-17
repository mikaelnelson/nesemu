#include "PpuDisplaySprite.h"

olc::Sprite* PpuDisplaySprite::draw(olc::PixelGameEngine* pge) {
  pge->SetDrawTarget(this);

  pge->Clear(olc::WHITE);
  // @todo Draw the PPU frame to the sprite

  pge->SetDrawTarget(nullptr);
  return this;
}

void PpuDisplaySprite::update(const PpuFrame& frame) { _frame = frame; }