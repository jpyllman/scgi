# catchorg/Clara single include
cmake_minimum_required(VERSION 3.11)
project(clara_lib LANGUAGES CXX)

add_library(clara INTERFACE)
target_include_directories(clara INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
