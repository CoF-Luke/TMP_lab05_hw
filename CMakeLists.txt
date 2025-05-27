cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)
option(COVERAGE "Enable coverage checking" OFF)

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
    target_link_libraries(check banking gtest_main gmock_main)
    add_test(NAME check COMMAND check)
endif()

if(COVERAGE)
    target_compile_options(check PRIVATE --coverage)
    target_link_options(check PRIVATE --coverage)
endif()