cmake_minimum_required (VERSION 3.23)

project ("DBusPermissionManager")

#find_package(sdbus-c++ REQUIRED)
#find_Package(sdbus-c++ REQUIRED)

#target_link_libraries(DBusPermissionManager PUBLIC sdbus-c++ SQLite)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY {CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY {CMAKE_BINARY_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY {CMAKE_BINARY_DIR}/lib)

add_subdirectory ("TimeService")
add_subdirectory ("PermissionsService")
add_subdirectory ("Client")
