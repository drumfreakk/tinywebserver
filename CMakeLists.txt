cmake_minimum_required(VERSION 3.15)
project(todo_webapp C)

set(CMAKE_C_STANDARD 99)

add_executable(todo_webapp main.c hacking-network.h hacking.h linked_list.h linked_list.c)