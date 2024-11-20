cmake_minimum_required(VERSION 3.23)

project(PermissionsService LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(sdbus-c++ REQUIRED)
find_package(SQLite3 REQUIRED)

set(SOURCES
    main.cpp
    PermissionsDatabase.cpp
    PermissionsService.cpp
)

set(HEADERS
    PermissionsDatabase.hpp
    PermissionsService.hpp
)

add_executable(PermissionsService ${SOURCES} ${HEADERS})

target_link_libraries(PermissionsService PRIVATE sdbus-c++ SQLite::SQLite3)

install(TARGETS PermissionsService DESTINATION /usr/bin)

