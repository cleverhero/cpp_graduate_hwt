#include "hwt.h"


namespace hwt {
    static int id_counter = 0;
    int next_tree_id() { return id_counter++; }
}