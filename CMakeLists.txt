cmake_minimum_required(VERSION 3.21.6)
project(blackjack)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_COMPILER clang)
set(CMAKE_BUILD_TYPE Debug)

IF (APPLE)
    set(LIBRARY_PATH /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib)
ENDIF (APPLE)

include_directories(include)

add_executable(blackjack
        src/blackjack.c
        src/main.c)

#add_compile_options(-g)

target_link_libraries(blackjack)

add_custom_command(TARGET blackjack
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:blackjack> ..)