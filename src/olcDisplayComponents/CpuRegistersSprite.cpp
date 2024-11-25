#include "CpuRegistersSprite.h"

olc::Sprite* CpuRegistersSprite::draw(olc::PixelGameEngine* pge) {
  static bool first_time = true;
  if (auto cpu_registers = _cpu->get_registers();
      cpu_registers != _prv_cpu_registers || first_time) {
    pge->SetDrawTarget(this);

    auto table = generate_table(cpu_registers);
    std::string table_str = "CPU Registers\n" + table.str();

    pge->Clear(olc::BLACK);
    pge->DrawString(4, 4, table_str, olc::WHITE);

    pge->SetDrawTarget(nullptr);
    _prv_cpu_registers = cpu_registers;
    first_time = false;
    return this;
  }

  return this;
}

tabulate::Table CpuRegistersSprite::generate_table(
    const Cpu::Registers& registers) {
  tabulate::Table table;

  std::string status_str = std::format(
      "{} {} {}\n\n{} {} {}", (registers.p_bits.negative ? "N" : "_"),
      (registers.p_bits.overflow ? "O" : "_"),
      (registers.p_bits.decimal ? "D" : "_"),
      (registers.p_bits.interrupt_disable ? "I" : "_"),
      (registers.p_bits.zero ? "Z" : "_"),
      (registers.p_bits.carry ? "C" : "_"));

  table.add_row({"Reg", "Value"});
  table.add_row({"A", std::format("{:02X}", registers.a)});
  table.add_row({"X", std::format("{:02X}", registers.x)});
  table.add_row({"Y", std::format("{:02X}", registers.y)});
  table.add_row({"SP", std::format("{:02X}", registers.sp)});
  table.add_row({"PC", std::format("{:04X}", registers.pc)});
  table.add_row({"P", status_str});

  return table;
}