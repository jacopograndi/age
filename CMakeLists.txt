cmake_minimum_required(VERSION 3.16)
project(sdl_test VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

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
    game/load.cpp game/load.h
    game/tech.cpp game/tech.h
    game/player.h
    game/constants.h
    game/ai/action.cpp game/ai/action.h
    game/ai/generator.h
    game/ai/engine.h
    game/ai/evaluator.h
    game/ai/tactic.h
    umath/vec2.h)

target_include_directories(sdl_test PUBLIC "/usr/local/include")
target_link_libraries(sdl_test -L/usr/local/libs)
target_link_libraries(sdl_test -lSDL2main -lSDL2)
