#include "num.h"

#include "gtest/gtest.h"

using namespace num;

TEST(types, binary) {
    const binary zero = 0;
    const binary one = 1;

    const binary zero1 = zero + zero;
    const binary zero2 = one + one;
    const binary zero3 = zero + 2;

    const binary one1 = zero + one;
    const binary one2 = one + zero;
    const binary one3 = one + 4;

    ASSERT_EQ(zero, zero1);
    ASSERT_EQ(zero, zero2);
    ASSERT_EQ(zero, zero3);
    ASSERT_EQ(one, one1);
    ASSERT_EQ(one, one2);
    ASSERT_EQ(one, one3);
    ASSERT_EQ(3, one3);
    ASSERT_EQ(-1, one3);
}

TEST(num, ternary) {
    const ternary zero = -15;
    const ternary one = -14;
    const ternary two = -13;
    
    ASSERT_EQ(0, zero);
    ASSERT_EQ(1, one);
    ASSERT_EQ(2, two);
    ASSERT_EQ(zero, one + two);
    ASSERT_EQ(two, one + one);
    ASSERT_EQ(one, two + two);
    ASSERT_EQ(zero, 3*one);
    ASSERT_EQ(2, 16*two);
    ASSERT_EQ(-2, -two);
    ASSERT_EQ(2, -one);
}

TEST(num, inverse) {
    constexpr int modulo = 101;
    for (int i = 1; i < modulo; i++) {
        const number<modulo> val = i;
        const number<modulo> inv = val.inverse();
        const number<modulo> prod = val * inv;
        ASSERT_EQ(i, val);
        ASSERT_EQ(1, prod);
    }
}
