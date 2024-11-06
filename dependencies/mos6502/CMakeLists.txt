message(STATUS "Fetching mos6502...")
FetchContent_MakeAvailable(mos6502)

target_include_directories(${PROJECT_NAME} PRIVATE ${mos6502_SOURCE_DIR})
target_sources(${PROJECT_NAME} PRIVATE ${mos6502_SOURCE_DIR}/mos6502.cpp)