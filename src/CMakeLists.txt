add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        CartridgeLoader.cpp
        CartridgeLoader.h
        NesCartridge.cpp
        NesCartridge.h
        MemoryMap.cpp
        MemoryMap.h
        Ram.cpp
        Ram.h
        Rom.cpp
        Rom.h
        Ppu.cpp
        Ppu.h
        Cpu.cpp
        Cpu.h
        IBus.h
        IMemory.h
        IObserver.h
        ISubject.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
