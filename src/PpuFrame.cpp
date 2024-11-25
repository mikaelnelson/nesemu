#include "PpuFrame.h"

#include "spdlog/spdlog.h"

void PpuFrame::set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g,
                         uint8_t b) {
  if (x >= WIDTH || y >= HEIGHT) {
    spdlog::error("PpuFrame::SetPixel: coordinates out of bounds ({}, {})", x,
                  y);
    return;
  }

  _data[WIDTH * y + x] = (r << 16) | (g << 8) | b;
}

uint32_t PpuFrame::get_pixel(uint8_t x, uint8_t y) const {
  if (x >= WIDTH || y >= HEIGHT) {
    spdlog::error("PpuFrame::GetPixel: coordinates out of bounds ({}, {})", x,
                  y);
    return 0;
  }

  return _data[WIDTH * y + x];
}