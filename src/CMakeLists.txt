add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_sources(${PROJECT_NAME} PRIVATE
        Clock.cpp
        Clock.h
        ClockDivider.cpp
        ClockDivider.h
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

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

add_subdirectory(olcDisplayComponents)
