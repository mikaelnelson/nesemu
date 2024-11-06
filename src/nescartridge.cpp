#include "nescartridge.h"
#include "spdlog/spdlog.h"

bool NesCartridge::load(MemoryMap &map) {
    const auto header = get_header();

    if (!header) {
        spdlog::error("NES Cartridge contains an invalid header");
        return false;
    }

    if (const auto prg_rom = get_prg_rom(*header)) {
        // @todo: Have base address and size for cartridge passed via
        // constructor. class itself will just know offsets within that address
        // space
        map.register_device(*prg_rom, 0x8000, (*prg_rom)->size());
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
    if (_data.size() < sizeof(Header)) {
        spdlog::error("ROM not large enough to contain header");
        return std::nullopt;
    }

    Header header;
    std::memcpy(&header, _data.data(), sizeof(Header));

    if (0 != memcmp(header.magic, good_magic, MAGIC_SIZE)) {
        spdlog::error("ROM Header contains invalid magic");
        return std::nullopt;
    }

    return header;
}

uint16_t NesCartridge::get_trainer_size(const Header &header) {
    return (header.flags_6.trainer ? TRAINER_SIZE : 0);
}

uint16_t NesCartridge::get_prg_rom_size(const Header &header) {
    return header.prg_rom_size * 16 * 1024;
}

std::optional<std::shared_ptr<Rom>>
NesCartridge::get_prg_rom(const Header &header) const {
    const uint16_t offset = sizeof(Header) + get_trainer_size(header);
    const uint16_t size = get_prg_rom_size(header);

    if (size == 0) {
        return std::nullopt;
    }

    return get_rom(offset, size);
}

uint16_t NesCartridge::get_chr_rom_size(const Header &header) {
    return header.chr_rom_size * 8 * 1024;
}

std::optional<std::shared_ptr<Rom>>
NesCartridge::get_chr_rom(const Header &header) const {
    const uint16_t offset = sizeof(Header) + get_trainer_size(header) +
                            get_prg_rom_size(header);
    const uint16_t size = get_chr_rom_size(header);

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
