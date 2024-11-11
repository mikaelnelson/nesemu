include(FetchContent)

# SPDLOG
FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
)
add_subdirectory(spdlog)

# mos6502
FetchContent_Declare(mos6502
        GIT_REPOSITORY https://github.com/gianlucag/mos6502.git
)
add_subdirectory(mos6502)

# OLC PixelGameEngine
add_subdirectory(olc)