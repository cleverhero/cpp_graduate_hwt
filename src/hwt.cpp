#include "nodes.h"
#include "binary_search_tree.h"
#include "avl_tree.h"
#include "splay_tree.h"
#include "order_statistic_tree.h"


namespace hwt {
    static int id_counter = 0;
    int next_tree_id() { return id_counter++; }
}