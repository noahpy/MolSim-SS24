
#include <iostream>
#include <gtest/gtest.h>

TEST(Test, test1){
    std::cout << "Test!" << std::endl;
    EXPECT_EQ(true, true);
}

