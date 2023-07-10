#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "hwt.h"
#include "config.h"


using std::filesystem::path;
using Config::RESOURCES_PATH;


TEST(HwtTests, Test_Big)
{
    path resources(RESOURCES_PATH);
    std::fstream testf, answerf;
    testf.open(resources / "big_gen_test.in", std::ios::in);
    answerf.open(resources /"big_gen_test.out", std::ios::in);

    if (!testf || !answerf) {
        std::cout << "NO SUCH FILE!!" << std::endl;
        ASSERT_TRUE(false);
    }

    hwt::OrderStatisticTree tree{};

    while (true) {
        char c;
        int i;

        testf >> c >> i;
        if (testf.eof())
            break;

        if (c == 'k') {
            tree.insert(i);
        }
        else if (c == 'n' || c == 'm') {
            int answer;
            answerf >> answer;

            if (testf.eof())
                ASSERT_TRUE(false);

            if (c == 'm')
                ASSERT_EQ(tree.select(i).value(), answer);
            else
                ASSERT_EQ(tree.rank(i), answer);
        }
    }

    testf.close();
    answerf.close();
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}