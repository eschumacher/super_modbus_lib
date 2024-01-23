cmake_minimum_required(VERSION 3.14.0)
project(super_modbus_lib VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_library(super_modbus_lib STATIC src/mb_function.cpp)
target_include_directories(super_modbus_lib
    SYSTEM
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
        message(WARNING "Must set CMAKE_BUILD_TYPE to Debug in order to build tests! Tests will not be built.")
    else()
        enable_testing()
        add_subdirectory(test)
        target_compile_options(super_modbus_lib PRIVATE --coverage)
        target_link_options(super_modbus_lib PUBLIC --coverage)
        add_custom_target(TARGET super_modbus_lib PRE_BUILD COMMAND find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +)
    endif()
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(examples)
