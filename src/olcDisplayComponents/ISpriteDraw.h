#pragma once

#include <olcPixelGameEngine.h>

class ISpriteDraw {
 public:
  virtual ~ISpriteDraw() = default;

  virtual olc::Sprite* draw(olc::PixelGameEngine* pge) = 0;
};
