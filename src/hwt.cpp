#include "hwt.h"
#include <iostream>


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
		else if (c == 'm')
			std::cout << tree.select(i)  << ' ';
    }

	return 0;
}