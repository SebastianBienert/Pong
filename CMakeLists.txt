cmake_minimum_required(VERSION 3.9)
project(PongV2)

set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp Ball.h Ball.cpp Paddle.h Paddle.cpp Constants.h)

SET(CMAKE_CXX_FLAGS -pthread)

add_executable(NCurses ${SOURCE_FILES})
target_link_libraries(NCurses ${CURSES_LIBRARY})