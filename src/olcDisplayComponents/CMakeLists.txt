target_include_directories(${PROJECT_NAME} PUBLIC .)
target_sources(${PROJECT_NAME} PUBLIC
        ISpriteDraw.h
        CpuStatusSprite.cpp
        CpuStatusSprite.h
        PpuDisplaySprite.cpp
        PpuDisplaySprite.h
        PpuRegistersSprite.cpp
        PpuRegistersSprite.h
)