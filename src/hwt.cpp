#include "hwt.h"


int main() {
	hwt::OrderStatisticTree tree{};
	tree.insert(8);
	tree.insert(2);
	tree.insert(-1);

	hwt::OrderStatisticTree tree2{};
	tree2 = tree;

	std::cout <<
		tree2.select(1) << " " <<
		tree2.select(2);

	std::cout << std::endl;

	std::cout << tree2.rank(3) << std::endl;

	return 0;
}