cmake_minimum_required(VERSION 3.12)
project(console)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "-lSDL2")
set (CMAKE_CXX_FLAGS "-lSDL2_ttf")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(console main.cpp Console/Console.h Control/Button.h Control/Canvas.h Handler/Handler.h Control/Control.h Handler/MouseLeftButtonDownHandler.h Handler/MouseLeftButtonUpHandler.h Handler/MouseOverHandler.h Handler/MouseOutHandler.h Utilities/Utilities.h)

target_link_libraries(console SDL2)
target_link_libraries(console SDL2_ttf)