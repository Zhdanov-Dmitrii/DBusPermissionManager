cmake_minimum_required(VERSION 3.10)

project(Client LANGUAGES CXX)
set(CMAKE_CXX_STANDART 17)

find_package(sdbus-c++ REQUIRED)

add_executable(Client main.cpp)
target_link_libraries(Client PRIVATE sdbus-c++)
