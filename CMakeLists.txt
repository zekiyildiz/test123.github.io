cmake_minimum_required(VERSION 3.29)
project(CENG209Homework C)

set(CMAKE_C_STANDARD 11)

add_executable(CENG209Homework game.c
        game.h
        main.c
        player.h
        player.c
        item.h
        item.c
        creature.c
        creature.h
        room.h
        room.c
)
