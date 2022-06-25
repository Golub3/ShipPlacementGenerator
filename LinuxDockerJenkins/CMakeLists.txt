cmake_minimum_required(VERSION 3.18)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
project(Lab1)
set(CMAKE_CXX_STANDARD 14)
add_executable(Lab1 task2.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lab1 PRIVATE Threads::Threads)


enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tester UnitTests.cpp)
target_link_libraries(tester ${GTEST_LIBRARIES} pthread)

gtest_discover_tests(tester)