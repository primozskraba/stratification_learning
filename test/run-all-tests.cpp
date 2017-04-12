#include "gtest/gtest.h"

/* #include "test-num.cpp" */

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}
