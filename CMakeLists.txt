cmake_minimum_required(VERSION 3.14.0)
project(super_modbus_lib VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(super_modbus_lib src/mb_hello.cpp)

target_include_directories(super_modbus_lib
    SYSTEM
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

enable_testing()
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
