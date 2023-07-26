#include <gtest/gtest.h>
#include "order_statistic_tree.h"
#include "splay_tree.h"


template<typename MT>
void handle(hwt::BinarySearchTree<MT>&& tree) {
    int k = 60;
    for (int i = 0; i < k; i++)
        tree.insert(i);

    for (int i = 0; i < k; i++)
        ASSERT_TRUE(tree.find(i));
}


TEST(HwtTests, Test_Templates)
{
    handle( hwt::BinarySearchTree{} );
    handle( hwt::BinarySearchTree<hwt::metadata::Full>{} );

    handle( hwt::AVLTree{} );

    handle( hwt::SplayTree{} );

    handle( hwt::OrderStatisticTree{} );
    handle( hwt::OrderStatisticTree<hwt::SplayTree>{} );
    handle( 
        hwt::OrderStatisticTree<
            hwt::BinarySearchTree,
            hwt::metadata::Full
        >{} 
    );
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}