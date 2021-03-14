cmake_minimum_required(VERSION 3.17)
project(lab01 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab01 zad2/main.c zad2/mylibrary.c zad2/mylibrary.h)
