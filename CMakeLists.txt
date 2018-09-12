cmake_minimum_required(VERSION 3.12)
project(neural_net)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(neural_net
        functions.h
        genetic.h
        main.cpp
        net.h
        SDLnet.h
        simulation.h)
