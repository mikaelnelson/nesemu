#pragma once

#include <array>
#include <cstdint>

class PpuFrame {
 public:
  static constexpr int WIDTH = 256;
  static constexpr int HEIGHT = 240;

  PpuFrame() = default;

  void set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
  uint32_t get_pixel(uint8_t x, uint8_t y) const;

 private:
  std::array<uint32_t, WIDTH * HEIGHT> _data;
};
