#include <iostream>
#include "order_statistic_tree.h"


int main() {
    hwt::OrderStatisticTree tree{};

    while (true) {
        char c;
        int i;
        std::cin >> c >> i;
        if (std::cin.eof())
            break;

        if (c == 'k') 
            tree.insert(i);
        else if (c == 'n')
            std::cout << tree.rank(i) << ' ';
        else if (c == 'm') {
            if (i <= 0) {
                std::cout << "Error in query: m " << i << std::endl;
                std::cout << "Expected value >= 1." << std::endl;
                continue;
            }
            auto answer = tree.select(i);
            if (answer)
                std::cout << answer.value() << ' ';
            else {
                std::cout << "Error in query: m " << i << std::endl;
                std::cout << "Can't find " << i << "th element." << std::endl;
            }
        }
    }

    return 0;
}