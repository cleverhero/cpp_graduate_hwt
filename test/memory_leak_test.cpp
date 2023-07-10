#include <gtest/gtest.h>
#include "hwt.h"


static int node_count = 0;


void* hwt::Node_::operator new(std::size_t n) {
    void *p = malloc(n);
    if (!p)
        throw std::bad_alloc{};
    node_count++;
    return p;
}


void hwt::Node_::operator delete(void * p) {
    free(p);
    node_count--;
}


TEST(HwtTests, Test_MemoryLeak)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 500000; i++)
        tree.insert(i);

    ASSERT_EQ(node_count, 500000);

    hwt::OrderStatisticTree cp_tree = tree;
    ASSERT_EQ(node_count, 1000000);

    ASSERT_EQ(cp_tree.select(1000), 999);
    ASSERT_EQ(cp_tree.rank(1000), 1000);

    for (int i = 0; i < 10; i++) {
        cp_tree = tree;
        ASSERT_EQ(node_count, 1000000);

        ASSERT_EQ(cp_tree.select(1000), 999);
        ASSERT_EQ(cp_tree.rank(1000), 1000);
    }

    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(node_count, 1000000);
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