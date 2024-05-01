cmake_minimum_required(VERSION 3.26)
project(net_3_parser_ini)

set(CMAKE_CXX_STANDARD 17)

add_executable(net_3_parser_ini main.cpp
                                ini_parser.h ini_parser.cpp)
