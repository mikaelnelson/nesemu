message(STATUS "Fetching Tabulate...")
FetchContent_MakeAvailable(tabulate)

target_include_directories(${PROJECT_NAME} PRIVATE "${tabulate_SOURCE_DIR}/single_include/")
