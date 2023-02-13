cmake_minimum_required(VERSION 3.14)
project(myChip8Sim VERSION 0.1.0)

enable_testing()

# download external dependencies

include(FetchContent)
FetchContent_Declare(
    sdl2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.26.3/SDL2-devel-2.26.3-mingw.zip
)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.1 # or a later release
)

FetchContent_MakeAvailable(sdl2 Catch2)

# add executable targets

add_executable(myChip8Sim 
    main.cpp
    Chip.cpp
    MyChip8Instructions.cpp
    MyChip8InstructionsGfxInput.cpp
    MyChipLogging.cpp
    MyGfx.cpp
    MyGfx.h
    MyKeyboardInput.cpp
)

add_executable(tests
    test/test.cpp
    Chip.cpp
    MyChip8Instructions.cpp
    MyChip8InstructionsGfxInput.cpp
    )

# copy SDL2.dll to build dir

add_custom_command(
    OUTPUT SDL2.dll
    DEPENDS "${sdl2_SOURCE_DIR}/x86_64-w64-mingw32/bin/SDL2.dll"
    COMMAND cp ${sdl2_SOURCE_DIR}/x86_64-w64-mingw32/bin/SDL2.dll ./
    VERBATIM
)
add_custom_target(copy_sdl2_dll DEPENDS SDL2.dll)
add_dependencies(myChip8Sim copy_sdl2_dll)

# configure include-dirs and lib-dirs

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_definitions(tests PUBLIC "UNIT_TEST")
target_include_directories(tests PRIVATE "./" "${sdl2_SOURCE_DIR}/x86_64-w64-mingw32/include/")

target_include_directories(myChip8Sim PRIVATE "${sdl2_SOURCE_DIR}/x86_64-w64-mingw32/include/")
target_link_directories(myChip8Sim PRIVATE "${sdl2_SOURCE_DIR}/x86_64-w64-mingw32/lib/")
target_link_libraries(myChip8Sim mingw32 SDL2main SDL2)

