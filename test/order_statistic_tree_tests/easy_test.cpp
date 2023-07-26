#include <gtest/gtest.h>
#include "order_statistic_tree.h"


TEST(HwtTests, Test_Rank1)
{
    hwt::OrderStatisticTree tree{};
    int k = 600;
    for (int i = 0; i < k; i++)
        tree.insert(i);

    for (int i = 0; i < k; i++)
        ASSERT_EQ(tree.rank(i), i);

    for (int i = 0; i < k; i++)
        ASSERT_EQ(tree.select(i + 1).value(), i);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}