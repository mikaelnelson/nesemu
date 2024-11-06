
#pragma once
#include <optional>
#include "cartridge.h"
#include "memorymap.h"
#include "ram.h"
#include "rom.h"

/**
 * NES Cartridge
 * File Format: https://www.nesdev.org/wiki/INES#iNES_file_format
 */
class NesCartridge : public Cartridge {
public:
    NesCartridge() = delete;
    explicit NesCartridge(std::vector<uint8_t> data) :
        Cartridge(std::move(data)) {};
    bool load(MemoryMap &map) override;

private:
    static constexpr uint16_t TRAINER_SIZE = 512;
    static constexpr uint8_t MAGIC_SIZE = 4;
    static constexpr uint8_t good_magic[MAGIC_SIZE] = {'N', 'E', 'S', 0x1A};
    struct Header {
        uint8_t magic[MAGIC_SIZE];
        uint8_t prg_rom_size;
        uint8_t chr_rom_size;
        struct Flags_6 {
            uint8_t lower_mapper_number : 4;
            uint8_t alt_nametable_lookup : 1;
            uint8_t trainer : 1;
            uint8_t battery_backed_prg_ram : 1;
            uint8_t nametable_arrangement : 1;
        } flags_6;
        struct Flags_7 {
            uint8_t upper_mapper_number : 4;
            uint8_t nes_format : 2;
            uint8_t playchoice_10 : 1;
            uint8_t vs_unisystem : 1;
        } flags_7;
        struct Flags_8 {
            uint8_t prg_ram_size;
        } flags_8;
        struct Flags_9 {
            uint8_t reserved : 7;
            uint8_t tv_system : 1;
        } flags_9;
        uint8_t flags_10; // Unsupported by NES format, generally unused
        uint8_t reserved[5];
    };
    static_assert(sizeof(Header) == 16, "Header must be 16 bytes");
    static_assert(std::is_trivially_copyable_v<Header>,
                  "Header must be trivially copyable");

    std::optional<Header> get_header() const;
    static uint16_t get_trainer_size(const Header &header);
    static uint16_t get_prg_rom_size(const Header &header);
    std::optional<std::shared_ptr<Rom>> get_prg_rom(const Header &header) const;
    static uint16_t get_chr_rom_size(const Header &header);
    std::optional<std::shared_ptr<Rom>> get_chr_rom(const Header &header) const;
    std::shared_ptr<Rom> get_rom(const uint16_t offset,
                                 const uint16_t size) const;
};