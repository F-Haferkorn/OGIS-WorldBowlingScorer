cmake_minimum_required(VERSION 3.11)

project(WorldBowlingScorer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(WorldBowlingScorer WIN32
  main.cpp
  bowlscorermainwindow.cpp
  bowlscorermainwindow.h
  bowlscorermainwindow.ui
  bowlscorer.h bowlscorer.cpp
  bowlplayerscore.h   bowlplayerscore.cpp
  bowlframescore.h  bowlframescore.cpp
)


target_link_libraries(WorldBowlingScorer PRIVATE Qt5::Widgets Qt5::Core)
