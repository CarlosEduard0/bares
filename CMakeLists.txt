cmake_minimum_required(VERSION 3.10)
project(bares)

set(CMAKE_CXX_STANDARD 11)

add_executable(bares src/main.cpp src/Parser.cpp include/Parser.h include/Token.h src/Simbolo.cpp include/Simbolo.h)