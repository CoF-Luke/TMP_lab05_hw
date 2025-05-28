cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 --coverage")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 --coverage")

option(BUILD_TESTS "Build tests" ON)
option(COVERAGE "Enable coverage checking" ON)

project(lab05hw)


add_subdirectory(third-party/gtest)

add_library(banking
    banking/Account.cpp
    banking/Transaction.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
)



if(COVERAGE)
    message(STATUS "Coverage build enabled")
    target_compile_options(banking PRIVATE -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(banking PRIVATE gcov)
endif()



add_executable(banking_tests
    tests/AccountTest.cpp
    tests/TransactionTest.cpp
)

target_include_directories(banking_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(banking_tests
    PRIVATE
    banking
    gtest
    gmock
    gtest_main
)

if(COVERAGE)
    target_compile_options(banking_tests PRIVATE -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(banking_tests PRIVATE gcov)
endif()

enable_testing()
add_test(NAME banking_tests COMMAND banking_tests)





