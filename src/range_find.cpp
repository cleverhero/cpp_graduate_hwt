#include "order_statistic_tree.h"
#include <iostream>


int main() {
    hwt::OrderStatisticTree tree{};

    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int key; std::cin >> key; 
        tree.insert(key);
    }

    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        int l, r; std::cin >> l >> r; 
        std::cout << tree.range_find(l, r) << ' ';
    }

    return 0;
}