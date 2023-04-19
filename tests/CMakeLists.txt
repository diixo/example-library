set(TARGET test_thread)
#set(TEST_TARGET test_library)

#include(AddGMock)
#add_gmock()
#include(AddGTestTest)

file(GLOB TEST_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    test_library.cpp
)

include_directories(
    "../include"
    "../src"
)

add_executable (${TARGET}                 "test_thread.cpp"        "LogInfo.cpp")
add_executable (test_thread_cv            "test_cv.cpp"            "LogInfo.cpp")
add_executable (test_thread_cv_wait_yield "test_cv_wait_yield.cpp" "LogInfo.cpp")
add_executable (test_thread_cv_wait       "test_cv_wait.cpp"       "LogInfo.cpp")
add_executable (test_thread_dispatch      "test_thread_dispatch.cpp" "LogInfo.cpp")

#enable_testing()
#add_gtest_test(${TEST_TARGET} "${TEST_SOURCE_FILES}" "")
