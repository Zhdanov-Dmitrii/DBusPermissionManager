cmake_minimum_required(VERSION 3.10)

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

target_link_libraries(PermissionsService PRIVATE sdbus-c++ SQLite)

install(TARGETS PermissionsService DESTINATION /usr/bin)

include(CPack)

set(CPACK_PACKAGE_NAME "permissions-service")
set(CPACK_PACKAGE_VENDOR "YourCompany")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DBus service for managing application permissions")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "support@yourcompany.com")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Utilities")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_REQUIRES "sdbus-c++, sqlite")

set(CPACK_GENERATOR "RPM")

