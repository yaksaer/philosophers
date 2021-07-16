cmake_minimum_required(VERSION 3.19)
project(philosophers C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(philosophers
        philo.c
        utils.c
        list_utils.c
        philosophers.h)
