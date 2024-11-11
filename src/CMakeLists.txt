add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        cartridgeloader.cpp
        nescartridge.cpp
        memorymap.cpp
        ram.cpp
        rom.cpp
        ppu.cpp
        cpu.cpp
)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

