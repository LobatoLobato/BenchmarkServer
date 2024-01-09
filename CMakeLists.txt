cmake_minimum_required(VERSION 3.27)
project(BenchmarkServer)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp)