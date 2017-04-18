#include "linalg.h"

#include "gtest/gtest.h"

using namespace la;

TEST(Vector, initializer_list) {
    TernaryVector x = { 1, 0, -2, -1, 3, 6, -3, -5 };

    ASSERT_EQ(1, x[0]);
    ASSERT_EQ(0, x[1]);
    ASSERT_EQ(1, x[2]);
    ASSERT_EQ(2, x[3]);
    ASSERT_EQ(0, x[4]);
    ASSERT_EQ(0, x[5]);
    ASSERT_EQ(0, x[6]);
    ASSERT_EQ(1, x[7]);
}

TEST(Vector, dot_product) {
    TernaryVector x = { 1, 0, 0, 2 };
    TernaryVector y = { 2, 1, 1, 0 };

    TernaryVector a = { 0, 1, 0, 1, 0, 1 };
    TernaryVector b = { 1, 0, 1, 0, 1, 0 };

    TernaryVector c = { 1, 1, 1, 1, 0, 1 };
    TernaryVector d = { 1, 0, 1, 0, 1, 0 };

    ASSERT_EQ(2, x*y);
    ASSERT_EQ(0, a*b);
    ASSERT_EQ(2, c*d);
}

TEST(Vector, sum) {
    TernaryVector a = { 1, 0, 0, 0, 2, 1, 2, 0 };
    TernaryVector b = { 1, 2, 0, 0, 1, 2, 1, 2 };
    TernaryVector c = { 2, 2, 0, 0, 0, 0, 0, 2 };


    TernaryVector x = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0 };
    TernaryVector y = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 2 };
    TernaryVector z = { 0, 1, 0, 0, 0, 2, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 2 };

    ASSERT_EQ(c, a + b);
    ASSERT_EQ(z, x + y);
}
