#include <gtest/gtest.h>
#include <iostream>
#include "order_statistic_tree.h"


static int allocation_counter = 0;
static int node_count = 0;

using hwt::metadata::Full;

template<>
void* hwt::Node_<Full>::operator new(std::size_t n) {
    if (++allocation_counter % 15  == 0)
        throw std::bad_alloc{};

    void *p = malloc(n);
    if (!p)
        throw std::bad_alloc{};
    node_count++;
    return p;
}

template<>
void hwt::Node_<Full>::operator delete(void * p) {
    free(p);
    node_count--;
}


TEST(HwtTests, Test_Insert)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 16; i++)       
        try {
            tree.insert(i);
        }
        catch (const std::bad_alloc& exc) {}

    ASSERT_EQ(node_count, 15);
}


TEST(HwtTests, Test_Copy_Constructor)
{
    hwt::OrderStatisticTree tree{};

    for (int i = 0; i < 10; i++)       
        tree.insert(i);

    hwt::OrderStatisticTree<>* p_tree = nullptr;
    try {
        p_tree = new hwt::OrderStatisticTree<>{tree};
    }
    catch (const std::bad_alloc& exc) {}

    ASSERT_EQ(node_count, 10);
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

    ASSERT_EQ(node_count, 10);
    ASSERT_EQ(cp_tree.nodes_count(), 0);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}