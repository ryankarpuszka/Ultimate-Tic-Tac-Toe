cmake_minimum_required(VERSION 3.14)
project(uttt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

add_library(game game.h game.cpp  ai.h ai.cpp)

add_executable(uttt main.cpp)
target_link_libraries(uttt game)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(uttt-gui gui.cpp gui.h)
target_link_libraries(uttt-gui sfml-graphics sfml-audio game)
configure_file(trim.ttf trim.ttf COPYONLY)

add_executable(game_test game_test.cpp)
target_link_libraries(
    game_test
    gtest_main
    game
)

add_executable(ai_test ai_test.cpp)
target_link_libraries(
    ai_test
    gtest_main
    game
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(GoogleTest)
gtest_discover_tests(game_test)
gtest_discover_tests(ai_test)