set(
    HEADERS_LIST 
    binary_search_tree.h
    avl_tree.h
    order_statistic_tree.h
    splay_tree.h
)

add_library(hwt_lib hwt.cpp ${HEADERS_LIST})

add_library(hwt_lib_test hwt.cpp ${HEADERS_LIST})
target_compile_definitions(hwt_lib_test PUBLIC "TEST_MEMORY")