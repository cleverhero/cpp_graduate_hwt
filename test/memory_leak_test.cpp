#include <gtest/gtest.h>
#include "../src/hwt.h"


TEST(HwtTests, Test_MemoryLeak)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 500000; i++)
        tree.insert(i);

    ASSERT_EQ(hwt::node_count, 500000);

    hwt::OrderStatisticTree cp_tree = tree;
    ASSERT_EQ(hwt::node_count, 1000000);

    ASSERT_EQ(cp_tree.select(1000), 999);
    ASSERT_EQ(cp_tree.rank(1000), 1000);

    for (int i = 0; i < 10; i++) {
        cp_tree = tree;
        ASSERT_EQ(hwt::node_count, 1000000);

        ASSERT_EQ(cp_tree.select(1000), 999);
        ASSERT_EQ(cp_tree.rank(1000), 1000);
    }

    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(hwt::node_count, 1000000);
        hwt::OrderStatisticTree cp_tree2 = tree;

        ASSERT_EQ(cp_tree2.select(1000), 999);
        ASSERT_EQ(cp_tree2.rank(1000), 1000);
    }
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}