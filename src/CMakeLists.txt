add_library(hwt_lib hwt.cpp binary_search_tree.h avl_tree.h order_statistic_tree.h)

add_library(hwt_lib_test hwt.cpp binary_search_tree.h avl_tree.h order_statistic_tree.h)
target_compile_definitions(hwt_lib_test PUBLIC "TEST_MEMORY")