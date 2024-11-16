#include "NesCartridge.h"

#include "spdlog/spdlog.h"

bool NesCartridge::load(MemoryMap &map) {
  const auto header = get_header();

  if (!header) {
    spdlog::error("NES Cartridge contains an invalid header");
    return false;
  }

  if (0 != header->get_mapper_number()) {
    spdlog::error("Only Mapper 0 is currently supported");
    return false;
  }

  if (const auto prg_rom = get_prg_rom(*header)) {
    // @todo: Have base address and size for cartridge passed via
    // constructor. class itself will just know offsets within that address
    // space
    uint16_t address = 0xFFFF - (*prg_rom)->size() + 1;

    spdlog::info("Map PRG ROM between 0x{:X} and 0x{:X}", address,
                 address + (*prg_rom)->size() - 1);
    map.register_device(*prg_rom, address, (*prg_rom)->size());
  } else {
    spdlog::error("NES Cartridge does not contain PRG ROM");
    return false;
  }

  if (const auto chr_rom = get_chr_rom(*header)) {
    // @todo: This needs to be mapped to 0x0000 of the PPU memory map
  }

  return true;
}

std::optional<NesCartridge::Header> NesCartridge::get_header() const {
  if (_data.size() < Header::get_header_size()) {
    spdlog::error("ROM not large enough to contain header");
    return std::nullopt;
  }

  std::array<uint8_t, Header::get_header_size()> header_data{};
  std::copy_n(_data.begin(), Header::get_header_size(), header_data.begin());

  Header header(header_data);

  if (!header.is_valid()) {
    spdlog::error("ROM Header is invalid");
    return std::nullopt;
  }

  return header;
}

std::optional<std::shared_ptr<Rom>> NesCartridge::get_prg_rom(
    const Header &header) const {
  const uint16_t offset = Header::get_header_size() + header.get_trainer_size();
  const uint16_t size = header.get_prg_rom_size();

  if (size == 0) {
    return std::nullopt;
  }

  return get_rom(offset, size);
}

std::optional<std::shared_ptr<Rom>> NesCartridge::get_chr_rom(
    const Header &header) const {
  const uint16_t offset = Header::get_header_size() +
                          header.get_trainer_size() + header.get_prg_rom_size();
  const uint16_t size = header.get_chr_rom_size();

  if (size == 0) {
    return std::nullopt;
  }

  return get_rom(offset, size);
}

std::shared_ptr<Rom> NesCartridge::get_rom(const uint16_t offset,
                                           const uint16_t size) const {
  std::vector<uint8_t> data(_data.begin() + offset,
                            _data.begin() + offset + size);

  return std::make_shared<Rom>(data);
}
