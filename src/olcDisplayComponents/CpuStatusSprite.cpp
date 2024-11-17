#include "CpuStatusSprite.h"

olc::Sprite* CpuStatusSprite::draw(olc::PixelGameEngine* pge) {
  if (auto cpu_status = _cpu->get_cpu_status(); cpu_status != _prv_cpu_status) {
    pge->SetDrawTarget(this);

    auto table = generate_table(cpu_status);

    pge->DrawString(0, 0, table.str(), olc::WHITE);

    pge->SetDrawTarget(nullptr);
    _prv_cpu_status = cpu_status;
    return this;
  }

  return nullptr;
}

tabulate::Table CpuStatusSprite::generate_table(const CpuStatus& status) {
  tabulate::Table table;

  table.add_row({"Reg", "Value"});
  table.add_row({"A", std::format("{:02X}", status.A)});
  table.add_row({"X", std::format("{:02X}", status.X)});
  table.add_row({"Y", std::format("{:02X}", status.Y)});
  table.add_row({"SP", std::format("{:02X}", status.SP)});
  table.add_row({"PC", std::format("{:04X}", status.PC)});
  table.add_row({"P", std::format("{:02X}", status.P)});

  return table;
}