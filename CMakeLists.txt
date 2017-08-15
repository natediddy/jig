cmake_minimum_required(VERSION 2.8)

project(Jig)


FILE(GLOB SOURCES *.cc)
add_executable(jig ${SOURCES})

set(CURSES_NEED_NCURSES TRUE)
find_package(curses REQUIRED)
target_link_libraries(jig "${CURSES_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS} -g -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
