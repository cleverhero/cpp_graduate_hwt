include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res")
get_filename_component(RESOURCES_PATH ${RESOURCES_PATH} ABSOLUTE)
configure_file(config.h.in config.h @ONLY)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(easy_test easy_test.cpp)
target_include_directories(easy_test PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(easy_test PRIVATE GTest::GTest hwt_lib)

add_executable(ml_test memory_leak_test.cpp)
target_include_directories(ml_test PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(ml_test PRIVATE GTest::GTest hwt_lib)

add_executable(little_test gen_test.cpp)
target_include_directories(little_test PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(little_test PRIVATE GTest::GTest hwt_lib)

add_executable(big_test big_gen_test.cpp)
target_include_directories(big_test PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(big_test PRIVATE GTest::GTest hwt_lib)


add_test(easy_test easy_test)
add_test(ml_test ml_test)
add_test(little_test little_test)
add_test(big_test big_test)