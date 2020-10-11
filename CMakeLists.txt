# cmake_minimum_required(VERSION 3.16)
project(Hash C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Hash
        lista.c
        hash.c
        hash_minipruebas.c)
