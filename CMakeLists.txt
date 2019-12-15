cmake_minimum_required(VERSION 3.10.2)
project(blackjack)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)

include_directories(include)

add_executable(blackjack
        src/blackjack.c
        src/main.c)

add_custom_command(TARGET blackjack
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:blackjack> ..)