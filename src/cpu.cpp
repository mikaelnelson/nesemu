#include "cpu.h"

Cpu::Cpu(std::shared_ptr<MemoryInterface> memory_map)
    : _memory_map(memory_map) {

}
