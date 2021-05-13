cmake_minimum_required(VERSION 3.5)


find_package(Qt5 COMPONENTS Core Gui Test REQUIRED)

#set(CMAKE_AUTOMOC ON)

enable_testing()

add_executable(ut_scorer testbowlscorer.h testbowlscorer.cpp)
add_test(NAME ut COMMAND ut_scorer)

target_include_directories(ut_scorer
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../LibWorldBowlingScorer/include
)


target_link_libraries(ut_scorer
    PRIVATE
    libWorldBowlingScorer
    Qt5::Widgets Qt::Gui Qt5::Core Qt5::Test
)

