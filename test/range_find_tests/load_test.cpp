#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "order_statistic_tree.h"
#include "config.h"


using std::filesystem::path;
using Config::RESOURCES_PATH;


TEST(HwtTests, Test_Big)
{
    path resources(RESOURCES_PATH);
    std::fstream testf, answerf;
    testf.open(resources / "range_find_test.in", std::ios::in);
    answerf.open(resources /"range_find_test.out", std::ios::in);

    if (!testf || !answerf) {
        std::cout << "NO SUCH FILE!!" << std::endl;
        ASSERT_TRUE(false);
    }

    hwt::OrderStatisticTree tree{};

    int n;
    testf >> n;
    for (int i = 0; i < n; i++) {
        int key; testf >> key; 
        tree.insert(key);
    }

    int m;
    testf >> m;
    for (int i = 0; i < m; i++) {
        int l, r; testf >> l >> r;
        int answer; answerf >> answer;
        ASSERT_EQ(tree.range_find(l, r), answer);
    }

    testf.close();
    answerf.close();
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}