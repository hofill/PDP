cmake_minimum_required(VERSION 3.22)
project(A1 C)

set(CMAKE_C_STANDARD 99)

add_executable(A1 main.c account.h account.c list.c list.h transfer.c transfer.h)
