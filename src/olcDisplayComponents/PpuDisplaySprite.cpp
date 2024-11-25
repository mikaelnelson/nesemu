#include "PpuDisplaySprite.h"

olc::Sprite* PpuDisplaySprite::draw(olc::PixelGameEngine* pge) { return this; }

void PpuDisplaySprite::update(const PpuFrame& frame) {
  for (int x = 0; x < PpuFrame::WIDTH; x++) {
    for (int y = 0; y < PpuFrame::HEIGHT; y++) {
      // Set Pixel, Ignore Alpha Channel
      SetPixel(x, y, olc::Pixel(0xFF000000 | frame.get_pixel(x, y)));
    }
  }
}