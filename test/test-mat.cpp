#include "structures.h"

#include "gtest/gtest.h"

using namespace la;

TEST(Matrix, initializer_list) {
    BinaryMatrix I = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    ASSERT_EQ(I(0,0), 1);
    ASSERT_EQ(I(0,1), 0);
    ASSERT_EQ(I(0,2), 0);
    
    ASSERT_EQ(I(1,0), 0);
    ASSERT_EQ(I(1,1), 1);
    ASSERT_EQ(I(1,2), 0);

    ASSERT_EQ(I(2,0), 0);
    ASSERT_EQ(I(2,1), 0);
    ASSERT_EQ(I(2,2), 1);
}
