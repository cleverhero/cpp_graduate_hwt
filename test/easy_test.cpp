#include <gtest/gtest.h>
#include "../src/hwt.h"


TEST(HwtTests, Test_Rank1)
{
    hwt::OrderStatisticTree tree{};
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    
    ASSERT_EQ(tree.rank(3), 2);
    ASSERT_EQ(tree.rank(9), 3);
    ASSERT_EQ(tree.rank(-2), 0);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}