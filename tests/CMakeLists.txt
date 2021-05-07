set(TARGET main_example)
set(TEST_TARGET test_library)

#include(AddGMock)
#add_gmock()

#include(AddGTestTest)

file(GLOB TEST_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    tst_library.cpp
)

include_directories(
    "../include"
    "../src"
)

add_executable (${TARGET} "main_example.cpp")

#enable_testing()
#add_gtest_test(${TEST_TARGET} "${TEST_SOURCE_FILES}" "")
