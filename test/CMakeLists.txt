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
set(os_tests order_statistic_tests)
set(rf_tests range_find_tests)

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

add_executable(os_functional_test ${os_tests}/functional_test.cpp)
target_include_directories(os_functional_test PUBLIC ${HEADERS_DIR})
target_link_libraries(os_functional_test PRIVATE GTest::GTest hwt_lib)

add_executable(os_load_test ${os_tests}/load_test.cpp)
target_include_directories(os_load_test PUBLIC ${CONFIG_DIR} ${HEADERS_DIR})
target_link_libraries(os_load_test PRIVATE GTest::GTest hwt_lib)

add_executable(rf_functional_test ${rf_tests}/functional_test.cpp)
target_include_directories(rf_functional_test PUBLIC ${HEADERS_DIR})
target_link_libraries(rf_functional_test PRIVATE GTest::GTest hwt_lib)

add_executable(rf_load_test ${rf_tests}/load_test.cpp)
target_include_directories(rf_load_test PUBLIC ${CONFIG_DIR} ${HEADERS_DIR})
target_link_libraries(rf_load_test PRIVATE GTest::GTest hwt_lib)

add_test(ml_test ml_test)
add_test(es_test es_test)
add_test(templates_test templates_test)

add_test(os_functional_test os_functional_test)
add_test(os_load_test os_load_test)

add_test(rf_functional_test rf_functional_test)
add_test(rf_load_test rf_load_test)
