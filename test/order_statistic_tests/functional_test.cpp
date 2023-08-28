#include <gtest/gtest.h>
#include "order_statistic_tree.h"
#include "splay_tree.h"


using namespace hwt::metadata;
const int K = 600;


template<
    template<typename MT>
    typename TreeType,
    typename MT
>
void rank_and_select(hwt::OrderStatisticTree<TreeType, MT>&& tree) {
    for (int i = 0; i < K; i++)
        ASSERT_EQ(tree.rank(i), i);

    for (int i = 0; i < K; i++)
        ASSERT_EQ(tree.select(i + 1).value(), i);
}


TEST(RankAndSelectTests, Test_AVL)
{
    hwt::OrderStatisticTree tree{};
    for (int i = 0; i < K; i++)
        tree.insert(i);

    rank_and_select(std::move(tree));
}


TEST(RankAndSelectTests, Test_Splay)
{
    hwt::OrderStatisticTree<hwt::SplayTree> tree{};
    for (int i = 0; i < K; i++)
        tree.insert(i);

    for (int i = 0; i < K; i++)
        tree.find(i);

    rank_and_select(std::move(tree));


    hwt::OrderStatisticTree<hwt::SplayTree, Size> tree_opt{};
    for (int i = 0; i < K; i++)
        tree_opt.insert(i);

    for (int i = 0; i < K; i++)
        tree_opt.find(i);

    rank_and_select(std::move(tree_opt));
}


TEST(RankAndSelectTests, Test_BST)
{
    hwt::OrderStatisticTree<hwt::BinarySearchTree> tree{};
    for (int i = 0; i < K; i++)
        tree.insert(i);

    rank_and_select(std::move(tree));


    hwt::OrderStatisticTree<hwt::BinarySearchTree, Size> tree_opt{};
    for (int i = 0; i < K; i++)
        tree_opt.insert(i);

    rank_and_select(std::move(tree_opt));
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}