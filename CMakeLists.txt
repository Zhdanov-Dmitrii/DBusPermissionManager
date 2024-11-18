cmake_minimum_required(VERSION 3.16)

# Имя основного проекта
project(DBusPermissionManager)

# Включаем C++17 (можно изменить на вашу версию, если требуется)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем подкаталоги для каждого бинарника
add_subdirectory(TimeService)
add_subdirectory(PermissionService)
add_subdirectory(Client)