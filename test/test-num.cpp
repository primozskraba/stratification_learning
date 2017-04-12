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

    ASSERT_EQ(zero1, zero);
    ASSERT_EQ(zero2, zero);
    ASSERT_EQ(zero3, zero);
    ASSERT_EQ(one1, one);
    ASSERT_EQ(one2, one);
    ASSERT_EQ(one3, one);
    ASSERT_EQ(one3, 3);
    ASSERT_EQ(one3, -1);
}

TEST(num, ternary) {
    const ternary zero = -15;
    const ternary one = -14;
    const ternary two = -13;
    
    ASSERT_EQ(zero, 0);
    ASSERT_EQ(one, 1);
    ASSERT_EQ(two, 2);
    ASSERT_EQ(one + two, zero);
    ASSERT_EQ(one + one, two);
    ASSERT_EQ(two + two, one);
    ASSERT_EQ(3*one, zero);
    ASSERT_EQ(16*two, 2);
    ASSERT_EQ(-two, -2);
    ASSERT_EQ(-one, 2);
}

TEST(num, inverse) {
    constexpr int modulo = 101;
    for (int i = 1; i < modulo; i++) {
        const number<modulo> val = i;
        const number<modulo> inv = val.inverse();
        const number<modulo> prod = val * inv;
        ASSERT_EQ(val, i);
        ASSERT_EQ(prod, 1);
    }
}
