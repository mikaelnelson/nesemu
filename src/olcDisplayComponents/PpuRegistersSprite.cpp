#include "PpuRegistersSprite.h"

olc::Sprite* PpuRegistersSprite::draw(olc::PixelGameEngine* pge) {
  static bool first_time = true;
  auto ppu_registers = _ppu->get_registers();
  auto ppu_int_registers = _ppu->get_internal_registers();

  if (ppu_registers != _prv_ppu_registers ||
      ppu_int_registers != _prv_int_ppu_registers || first_time) {
    pge->SetDrawTarget(this);

    auto reg_table = generate_registers_table(ppu_registers);
    auto int_reg_table = generate_internal_registers_table(ppu_int_registers);
    std::string table_str = "PPU Registers\n" + reg_table.str() +
                            "\n\nPPU Int Registers\n" + int_reg_table.str();

    pge->Clear(olc::BLACK);
    pge->DrawString(4, 4, table_str, olc::WHITE);

    pge->SetDrawTarget(nullptr);
    _prv_ppu_registers = ppu_registers;
    _prv_int_ppu_registers = ppu_int_registers;
    first_time = false;
    return this;
  }

  return this;
}

tabulate::Table PpuRegistersSprite::generate_registers_table(
    Ppu::Registers& registers) {
  tabulate::Table table;

  table.add_row({"Reg", "Value"});
  table.add_row({"CTRL", std::format("{:02X}", registers.controller)});
  table.add_row({"MASK", std::format("{:02X}", registers.mask)});
  table.add_row({"STATUS", std::format("{:08B}", registers.status)});
  table.add_row({"OAMA", std::format("{:02X}", registers.oam_address)});
  table.add_row({"OAMD", std::format("{:02X}", registers.oam_data)});
  table.add_row({"SCROLL", std::format("{:02X}", registers.scroll)});
  table.add_row({"ADDR", std::format("{:04X}", registers.address)});
  table.add_row({"DATA", std::format("{:02X}", registers.data)});

  return table;
}
tabulate::Table PpuRegistersSprite::generate_internal_registers_table(
    Ppu::InternalRegisters& registers) {
  tabulate::Table table;

  table.add_row({"Reg", "Value"});
  table.add_row(
      {"W", std::format("{:01B}", static_cast<uint8_t>(registers.w))});

  return table;
}