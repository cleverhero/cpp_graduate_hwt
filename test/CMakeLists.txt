include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
get_filename_component(RESOURCES_PATH ${RESOURCES_PATH} ABSOLUTE)
configure_file(config.h.in config.h @ONLY)

set(CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

set(base_tests base_tests)
set(ost_tests order_statistic_tree_tests)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(es_test ${base_tests}/exception_safety_test.cpp)
target_include_directories(es_test PUBLIC ${HEADERS_DIR})
target_link_libraries(es_test PRIVATE GTest::GTest hwt_lib_test)

add_executable(ml_test ${base_tests}/memory_leak_test.cpp)
target_include_directories(ml_test PUBLIC ${HEADERS_DIR})
target_link_libraries(ml_test PRIVATE GTest::GTest hwt_lib_test)

add_executable(templates_test ${base_tests}/templates_test.cpp)
target_include_directories(templates_test PUBLIC ${HEADERS_DIR})
target_link_libraries(templates_test PRIVATE GTest::GTest hwt_lib)

add_executable(easy_test ${ost_tests}/easy_test.cpp)
target_include_directories(easy_test PUBLIC ${HEADERS_DIR})
target_link_libraries(easy_test PRIVATE GTest::GTest hwt_lib)

add_executable(big_test ${ost_tests}/big_gen_test.cpp)
target_include_directories(big_test PUBLIC ${CONFIG_DIR} ${HEADERS_DIR})
target_link_libraries(big_test PRIVATE GTest::GTest hwt_lib)

add_test(ml_test ml_test)
add_test(es_test es_test)
add_test(templates_test templates_test)

add_test(easy_test easy_test)
add_test(big_test big_test)
