add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        cartridgeloader.cpp
        nescartridge.cpp
        memorymap.cpp
        ram.cpp
        rom.cpp
        cpu.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
