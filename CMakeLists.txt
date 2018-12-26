cmake_minimum_required(VERSION 3.13)
project(neural_net)

set(CMAKE_CXX_STANDARD 14)

include_directories(. ;
        C:\\Users\\emili\\Programmazione\\Librerie\\SDL2\\include ;
        C:\\Users\\emili\\Programmazione\\Librerie\\SDL2_image\\include ;
        C:\\Users\\emili\\Programmazione\\Librerie\\SDL2_ttf\\include
        )

add_executable(neural_net
        functions.h
        genetic.h
        main.cpp
        net.h
        pch.cpp
        pch.h
        SDLnet.h
        simulation.h
        )

link_directories(
        C:\\Users\\emili\\Programmazione\\Librerie\\SDL2\\lib ;
        C:\\Users\\emili\\Programmazione\\Librerie\\SDL2_image\\lib ;
        C:\\Users\\emili\\Programmazione\\Librerie\\SDL2_ttf\\lib
)
