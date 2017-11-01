cmake_minimum_required(VERSION 3.7)
project(mambo)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SFML COMPONENTS graphics window system)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-std=c++1z)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-Wall)
endif()

option(sanitize "compile with sanitizers" OFF)
if(sanitize AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-sanitize-recover=undefined,integer")
endif()


include_directories(extern)
include_directories(src)

#add_definitions(-flto=thin)

set(SOURCE_FILES
    src/GameObject.cpp
    src/GameObject.h
    src/gamedata/GameData.cpp
    src/gamedata/GameData.h
    src/gamedata/TileData.cpp
    src/gamedata/TileData.h
    src/Logger.cpp
    src/Logger.h
    src/Character.cpp
    src/Character.h
    src/Vec2.cpp
    src/Vec2.h
    src/gamedata/TextureAtlas.cpp
    src/gamedata/TextureAtlas.h
    src/level/LevelGen.cpp
    src/level/LevelGen.h
    src/level/Level.cpp
    src/level/Level.h
    src/level/Tile.cpp
    src/level/Tile.h
    src/level/TileMap.cpp
    src/level/TileMap.h
    src/controls/PlayerControls.cpp
    src/controls/PlayerControls.h
    src/gamedata/ItemData.cpp
    src/gamedata/ItemData.h
    src/Controlable.h
    src/Controlable.cpp
    src/stb_perlin.h
    src/stb_perlin.cpp
    src/Item.h
    src/Item.cpp
)
add_library(argos_core ${SOURCE_FILES})
target_link_libraries(argos_core ${SFML_LIBRARIES})

add_executable(argos main.cpp)
target_link_libraries(argos argos_core)

add_subdirectory(unittests)
