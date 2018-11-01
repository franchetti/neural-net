cmake_minimum_required(VERSION 3.12)
project(neural_net)

set(CMAKE_CXX_STANDARD 17)

add_executable(neural_net main.cpp functions.h genetic.h net.h SDLnet.h simulation.h)