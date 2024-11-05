include(FetchContent)

# SPDLOG
FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
)
add_subdirectory(spdlog)