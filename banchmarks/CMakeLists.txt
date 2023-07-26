set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/)


add_executable(range_find_avltree range_find_avltree.cpp)
target_include_directories(range_find_avltree PUBLIC ${HEADERS_DIR})
target_link_libraries(range_find_avltree hwt_lib)