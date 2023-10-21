
include(FetchContent)

# GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
)
# Recommended by GoogleTest docs to support Windows environments
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(super_modbus_tests
    gtest_test.cpp
    rtu_parser_read_hr.cpp
    rtu_parser_read_ir.cpp
)

target_link_libraries(super_modbus_tests
    super_modbus_lib
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(super_modbus_tests)
