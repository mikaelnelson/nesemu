add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        CartridgeLoader.cpp
        NesCartridge.cpp
        MemoryMap.cpp
        Ram.cpp
        Rom.cpp
        Ppu.cpp
        Cpu.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
