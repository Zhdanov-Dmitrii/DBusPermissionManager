cmake_minimum_required(VERSION 3.10)

project(TimeService LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(sdbus-c++ REQUIRED)
find_package(SQLite3 REQUIRED)

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

include(CPack)
set(CPACK_PACKAGE_NAME "time-service")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "RPM")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DBus service for accessing system time with permissions")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "support@yourcompany.com")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Utilities")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_REQUIRES "sdbus-c++")

