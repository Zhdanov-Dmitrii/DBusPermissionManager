cmake_minimum_required(VERSION 3.23)

project(TimeService LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(sdbus-c++ REQUIRED)
#find_package(SQLite3 REQUIRED)

set(SOURCES
    main.cpp
    TimeService.cpp
)

set(HEADERS
    TimeService.hpp
)

add_executable(TimeService ${SOURCES} ${HEADERS})
target_link_libraries(TimeService PRIVATE sdbus-c++)

install(TARGETS TimeService DESTINATION /usr/bin)
