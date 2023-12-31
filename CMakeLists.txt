cmake_minimum_required(VERSION 3.23)
project(llp_lab2 C)

find_package(FLEX)
find_package(BISON)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} main.c parser/ast.c parser/ast.h)
