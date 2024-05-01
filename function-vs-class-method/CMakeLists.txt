cmake_minimum_required(VERSION 3.23)
project(yandex_practicum)

set(CMAKE_CXX_FLAGS -Wall)

set(CMAKE_CXX_STANDARD 17)

add_executable(
		${PROJECT_NAME}
		ebook.cpp
)
