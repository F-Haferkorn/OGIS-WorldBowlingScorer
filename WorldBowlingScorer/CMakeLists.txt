add_executable(WorldBowlingScorer
    WIN32
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(WorldBowlingScorer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../LibWorldBowlingScorer/include)

target_link_libraries(WorldBowlingScorer
    PRIVATE libWorldBowlingScorer Qt5::Widgets Qt::Gui Qt5::Core)


