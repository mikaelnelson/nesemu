add_executable(${PROJECT_NAME} main.cpp memorymap.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
