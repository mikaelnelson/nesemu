#include "CpuRegistersSprite.h"

olc::Sprite* CpuRegistersSprite::draw(olc::PixelGameEngine* pge) {
  static bool first_time = true;
  if (auto cpu_status = _cpu->get_cpu_status();
      cpu_status != _prv_cpu_status || first_time) {
    pge->SetDrawTarget(this);

    auto table = generate_table(cpu_status);
    std::string table_str = "CPU Registers\n" + table.str();

    pge->Clear(olc::BLACK);
    pge->DrawString(4, 4, table_str, olc::WHITE);

    pge->SetDrawTarget(nullptr);
    _prv_cpu_status = cpu_status;
    first_time = false;
    return this;
  }

  return this;
}

tabulate::Table CpuRegistersSprite::generate_table(const CpuStatus& status) {
  tabulate::Table table;

  table.add_row({"Reg", "Value"});
  table.add_row({"A", std::format("{:02X}", status.A)});
  table.add_row({"X", std::format("{:02X}", status.X)});
  table.add_row({"Y", std::format("{:02X}", status.Y)});
  table.add_row({"SP", std::format("{:02X}", status.SP)});
  table.add_row({"PC", std::format("{:04X}", status.PC)});
  table.add_row({"P", std::format("{:08B}", status.P)});

  return table;
}