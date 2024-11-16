#include "CpuStatusSprite.h"

olc::Sprite* CpuStatusSprite::draw(olc::PixelGameEngine* pge) {
  static CpuStatus last_cpu_status;
  if (auto cpu_status = _cpu->get_cpu_status(); cpu_status != last_cpu_status) {
    pge->SetDrawTarget(this);

    pge->FillRect(0, 0, width, height, olc::GREEN);

    pge->SetDrawTarget(nullptr);
    last_cpu_status = cpu_status;
    return this;
  }

  return nullptr;
}
