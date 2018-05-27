cmake_minimum_required(VERSION 3.7)
project(mambo)

enable_testing()

find_package(SFML COMPONENTS graphics window system REQUIRED)

set(SFML_LIBRARIES sfml-audio sfml-graphics sfml-window sfml-system)

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
    src/combat/Projectile.h
    src/combat/Projectile.cpp
    src/gamedata/GameData.cpp
    src/gamedata/GameData.h
    src/gamedata/TileData.cpp
    src/gamedata/TileData.h
    src/gamedata/ProjectileData.cpp
    src/gamedata/ProjectileData.h
    src/Logger.cpp
    src/Logger.h
    src/Attribute.cpp
    src/Attribute.h
    src/SpellEffect.cpp
    src/SpellEffect.h
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
    src/level/TileExtraInfo.cpp
    src/level/TileExtraInfo.h
    src/level/TileMap.cpp
    src/level/TileMap.h
    src/level/World.h
    src/level/World.cpp
    src/level/WorldGen.h
    src/level/WorldGen.cpp
    src/controls/PlayerControls.cpp
    src/controls/PlayerControls.h
    src/controls/GameUI.cpp
    src/controls/GameUI.h
    src/gamedata/ItemData.cpp
    src/gamedata/ItemData.h
    src/Controlable.h
    src/Controlable.cpp
    src/stb_perlin.h
    src/stb_perlin.cpp
    src/Item.h
    src/Item.cpp
    src/stlastar.h
    src/fsa.h
    src/Perlin2D.h
    src/PathFinder.h
    src/PathFinder.cpp
    src/Faction.h
    src/Faction.cpp
    src/Creature.h
    src/Creature.cpp
    src/TilePos.h
    src/TilePos.cpp
    src/Inventory.cpp
    src/Inventory.h
    src/gamedata/RandomRange.cpp
    src/gamedata/RandomRange.h
    src/level/TileRect.cpp
    src/level/TileRect.h
    src/ItemBag.cpp
    src/ItemBag.h
)
add_library(argos_core ${SOURCE_FILES})
target_link_libraries(argos_core ${SFML_LIBRARIES})

add_executable(argos main.cpp)
target_link_libraries(argos argos_core)

add_subdirectory(unittests)
