cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 --coverage")

option(BUILD_TESTS "Build tests" ON)
option(COVERAGE "Enable coverage checking" ON)

project(lab05hw)


add_library(banking
    banking/Account.cpp
    banking/Transaction.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
)


if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)
    file(GLOB BANKING_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    add_executable(check ${BANKING_TEST_SOURCES})
    target_link_libraries(check banking gtest gmock gtest_main gmock_main)
    add_test(NAME check COMMAND check)
endif()

if(COVERAGE)
    target_compile_options(banking PRIVATE -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(banking PRIVATE gcov)
    target_compile_options(check PRIVATE -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(check PRIVATE gcov)
endif()