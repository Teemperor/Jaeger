cmake_minimum_required(VERSION 3.7)
project(mambo)

#add_definitions(-fmodules -fimplicit-module-maps -fmodules-local-submodule-visibility)

set(CMAKE_CXX_STANDARD 11)

include_directories(extern)
include_directories(src)

#add_definitions(-flto=thin)

set(SOURCE_FILES main.cpp
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
)
add_executable(argos ${SOURCE_FILES})
target_link_libraries(argos sfml-graphics sfml-system sfml-window Box2D)
