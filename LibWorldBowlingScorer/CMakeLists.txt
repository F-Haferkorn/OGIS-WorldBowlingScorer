cmake_minimum_required(VERSION 3.11)


#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

add_library(libWorldBowlingScorer STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ogis/wbscorer/bowlscorermainwindow.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bowlscorermainwindow.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/ogis/wbscorer/bowlscorer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bowlscorer.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/ogis/wbscorer/bowlplayerscore.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bowlplayerscore.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/ogis/wbscorer/bowlframescore.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bowlframescore.cpp

)

target_include_directories(
    libWorldBowlingScorer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(libWorldBowlingScorer
    PRIVATE Qt5::Widgets Qt::Gui Qt5::Core)

