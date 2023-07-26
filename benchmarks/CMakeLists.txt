set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/)


add_executable(avl_vs_splay avl_vs_splay.cpp)
target_include_directories(avl_vs_splay PUBLIC ${HEADERS_DIR})
target_link_libraries(avl_vs_splay hwt_lib)