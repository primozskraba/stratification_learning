#include "linalg.h"

#include "gtest/gtest.h"

using namespace la;

TEST(Matrix, initializer_list) {
    BinaryMatrix I = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    ASSERT_EQ(1, I(0,0));
    ASSERT_EQ(0, I(0,1));
    ASSERT_EQ(0, I(0,2));

    ASSERT_EQ(0, I(1,0));
    ASSERT_EQ(1, I(1,1));
    ASSERT_EQ(0, I(1,2));

    ASSERT_EQ(0, I(2,0));
    ASSERT_EQ(0, I(2,1));
    ASSERT_EQ(1, I(2,2));

    TernaryMatrix A {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, 1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };

    ASSERT_EQ(MatrixEntry<ternary>({1,1}), A(0,0));
    ASSERT_EQ(MatrixEntry<ternary>({-1,2}), A(0,1));
    ASSERT_EQ(0, A(0,2));

    ASSERT_EQ(MatrixEntry<ternary>({-1,-2}), A(1,0));
    ASSERT_EQ(0, A(1,1));
    ASSERT_EQ(MatrixEntry<ternary>({1,1}), A(1,2));

    ASSERT_EQ(0, A(2,0));
    ASSERT_EQ(MatrixEntry<ternary>({1,-2}), A(2,1));
    ASSERT_EQ(MatrixEntry<ternary>({-1,-1}), A(2,2));
}

TEST(Matrix, vector_constructor) {
    BinaryVector b0 = { 1, 0, 1 };
    BinaryVector b1 = { 0, 1, 1 };
    
    BinaryMatrix A { b0, b1 };

    ASSERT_EQ(1, A(0,0));
    ASSERT_EQ(0, A(1,0));
    ASSERT_EQ(1, A(2,0));
    ASSERT_EQ(0, A(0,1));
    ASSERT_EQ(1, A(1,1));
    ASSERT_EQ(1, A(2,1));
}

TEST(Matrix, transpose) {
    TernaryMatrix A = {
        {2, 1, 0},
        {0, 2, 1},
        {1, 1, 1}
    };
    BinaryMatrix B = {
        {1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1}
    };

    TernaryMatrix At = transpose(A);
    BinaryMatrix Bt = transpose(B);

    ASSERT_EQ(A(0,0), At(0,0));
    ASSERT_EQ(A(0,1), At(1,0));
    ASSERT_EQ(A(0,2), At(2,0));

    ASSERT_EQ(A(1,0), At(0,1));
    ASSERT_EQ(A(1,1), At(1,1));
    ASSERT_EQ(A(1,2), At(2,1));

    ASSERT_EQ(A(2,0), At(0,2));
    ASSERT_EQ(A(2,1), At(1,2));
    ASSERT_EQ(A(2,2), At(2,2));

    ASSERT_EQ(3, At.rows());
    ASSERT_EQ(3, At.cols());

    ASSERT_EQ(6, Bt.rows());
    ASSERT_EQ(2, Bt.cols());

    ASSERT_EQ(B(0,0), Bt(0,0));
    ASSERT_EQ(B(0,1), Bt(1,0));
    ASSERT_EQ(B(0,2), Bt(2,0));
    ASSERT_EQ(B(0,3), Bt(3,0));
    ASSERT_EQ(B(0,4), Bt(4,0));
    ASSERT_EQ(B(0,5), Bt(5,0));

    ASSERT_EQ(B(1,0), Bt(0,1));
    ASSERT_EQ(B(1,1), Bt(1,1));
    ASSERT_EQ(B(1,2), Bt(2,1));
    ASSERT_EQ(B(1,3), Bt(3,1));
    ASSERT_EQ(B(1,4), Bt(4,1));
    ASSERT_EQ(B(1,5), Bt(5,1));

    // TODO test for time matrices
}

TEST(Matrix, multiply) {
    BinaryMatrix I = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    TernaryMatrix A = {
        { 2, 2, 0, 2, 2 },
        { 2, 1, 1, 0, 1 },
        { 0, 0, 2, 2, 2 }
    };
    TernaryMatrix B = {
        { 0, 1, 2, 2, 2, 1 },
        { 1, 0, 2, 0, 2, 1 },
        { 2, 2, 1, 0, 0, 2 },
        { 2, 2, 1, 0, 2, 2 },
        { 2, 2, 0, 0, 0, 0 }
    };
	TernaryMatrix AB_expected = {
        { 1, 1, 1, 1, 0, 2 },
        { 2, 0, 1, 1, 0, 2 },
        { 0, 0, 1, 0, 1, 2 }
    };
    TernaryMatrix AB = A * B;

    ASSERT_EQ(I, I*I);
    ASSERT_EQ(3, (I*I).rows());
    ASSERT_EQ(3, (I*I).cols());

    ASSERT_EQ(AB_expected(0,0), AB(0,0));
    ASSERT_EQ(AB_expected(0,1), AB(0,1));
    ASSERT_EQ(AB_expected(0,2), AB(0,2));
    ASSERT_EQ(AB_expected(0,3), AB(0,3));
    ASSERT_EQ(AB_expected(0,4), AB(0,4));
    ASSERT_EQ(AB_expected(0,5), AB(0,5));

    ASSERT_EQ(AB_expected(1,0), AB(1,0));
    ASSERT_EQ(AB_expected(1,1), AB(1,1));
    ASSERT_EQ(AB_expected(1,2), AB(1,2));
    ASSERT_EQ(AB_expected(1,3), AB(1,3));
    ASSERT_EQ(AB_expected(1,4), AB(1,4));
    ASSERT_EQ(AB_expected(1,5), AB(1,5));

    ASSERT_EQ(AB_expected(2,0), AB(2,0));
    ASSERT_EQ(AB_expected(2,1), AB(2,1));
    ASSERT_EQ(AB_expected(2,2), AB(2,2));
    ASSERT_EQ(AB_expected(2,3), AB(2,3));
    ASSERT_EQ(AB_expected(2,4), AB(2,4));
    ASSERT_EQ(AB_expected(2,5), AB(2,5));

    ASSERT_EQ(3, AB.rows());
    ASSERT_EQ(6, AB.cols());

    // TODO test for time matrices
}
