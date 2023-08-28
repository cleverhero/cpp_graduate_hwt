#include <gtest/gtest.h>
#include "order_statistic_tree.h"
#include "splay_tree.h"


using namespace hwt::metadata;
const int K = 600;


template<typename MT>
void range_find_test(hwt::BinarySearchTree<MT>&& tree) {
    for (int i = 0; i < K; i++)
        tree.insert(i);

    int m = K / 4;
    ASSERT_EQ(tree.range_find(-K, 0), 0);
    ASSERT_EQ(tree.range_find(0, K), K);
    ASSERT_EQ(tree.range_find(m, K), K - m);
    ASSERT_EQ(tree.range_find(K, 2*K), 0);
}


TEST(RangeFindTests, AllTests)
{
    range_find_test(hwt::BinarySearchTree{});
    range_find_test(hwt::SplayTree{});
    range_find_test(hwt::AVLTree{});
    range_find_test(hwt::OrderStatisticTree{});
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}