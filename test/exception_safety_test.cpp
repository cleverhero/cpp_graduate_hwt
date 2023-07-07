#include <gtest/gtest.h>
#include "../src/hwt.h"
#include <iostream>


TEST(HwtTests, Test_Insert)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 16; i++)       
        try {
            tree.insert(i);
        }
        catch (const std::bad_alloc& exc) {}

    ASSERT_EQ(hwt::get_node_count(), 15);
}


TEST(HwtTests, Test_Copy_Constructor)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 10; i++)       
        tree.insert(i);

    hwt::OrderStatisticTree* p_tree = nullptr;
    try {
        p_tree = new hwt::OrderStatisticTree{tree};
    }
    catch (const std::bad_alloc& exc) {}

    ASSERT_EQ(hwt::get_node_count(), 10);
    ASSERT_EQ(p_tree, nullptr);
}


TEST(HwtTests, Test_Copy_Assignment)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 10; i++)       
        tree.insert(i);

    hwt::OrderStatisticTree cp_tree{};
    try {
        cp_tree = tree;
    }
    catch (const std::bad_alloc& exc) {}

    ASSERT_EQ(hwt::get_node_count(), 10);
    ASSERT_EQ(cp_tree.nodes.size(), 0);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}