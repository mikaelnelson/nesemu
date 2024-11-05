#include <spdlog/spdlog.h>
#include "memorymap.h"
#include "ram.h"

int main() {
    auto memory_map = std::make_shared<MemoryMap>();

    auto internal_ram = std::make_shared<Ram>(0x0800);
    
    // Register Internal Ram
    memory_map->register_device(internal_ram, 0x0000, internal_ram->size());

    // Register Internal Ram Mirrors
    memory_map->register_device(internal_ram, 0x0800, internal_ram->size());
    memory_map->register_device(internal_ram, 0x1000, internal_ram->size());
    memory_map->register_device(internal_ram, 0x1800, internal_ram->size());

    return 0;
}
