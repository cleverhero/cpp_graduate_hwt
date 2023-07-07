add_library(hwt_lib hwt.cpp hwt.h)

add_library(hwt_lib_test hwt.cpp hwt.h)
target_compile_definitions(hwt_lib_test PRIVATE "TEST_EXCEPTION_SAFETY")