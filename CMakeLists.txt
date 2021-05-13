cmake_minimum_required(VERSION 3.11)

project(WorldBowlingScorer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



option("WBOWL_UNIT_TESTS" "make unit unit tests" ON)

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)



add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LibWorldBowlingScorer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/WorldBowlingScorer)

if(WBOWL_UNIT_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/unit_tests)
endif()
