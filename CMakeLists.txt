cmake_minimum_required(VERSION 3.16)
project(HWT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmarks)

add_executable(order_statistic src/order_statistic.cpp)
target_link_libraries(order_statistic hwt_lib)

add_executable(range_find src/range_find.cpp)
target_link_libraries(range_find hwt_lib)