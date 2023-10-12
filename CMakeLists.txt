cmake_minimum_required(VERSION 3.0.0)
project(super_modbus_lib VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

add_library(super_modbus_lib src/mb_hello.cpp)

add_executable(super_modbus_test test/test.cpp)

target_include_directories(super_modbus_lib
    SYSTEM
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(super_modbus_test
    super_modbus_lib
)

target_include_directories(super_modbus_test
    SYSTEM
    PRIVATE
    super_modbus_lib
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
