cmake_minimum_required(VERSION 3.27)
project(yandex_practicum)

set(CMAKE_CXX_FLAGS -Wall)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ${PROJECT_NAME}
        main.cpp
)

