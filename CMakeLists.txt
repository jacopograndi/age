cmake_minimum_required(VERSION 3.16)
project(sdl_test VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)

add_executable(sdl_test 
    main.cpp
    graphics/graphics.cpp graphics/graphics.h
    graphics/cam.h
    commands/commands.cpp commands/commands.h
    timing/timing.cpp timing/timing.h
    game/entity.cpp game/entity.h
    game/ground.cpp game/ground.h
    game/playercontrol.cpp game/playercontrol.h
    game/gst.cpp game/gst.h
    game/view.cpp game/view.h
    game/menu.cpp game/menu.h
    game/tile.cpp game/tile.h
    game/player.h
    umath/vec2.h)

target_include_directories(sdl_test PUBLIC "C:/MinGW_libs/include")
target_link_libraries(sdl_test -LC:/MinGW_libs/lib)
target_link_libraries(sdl_test -lSDL2main -lSDL2)