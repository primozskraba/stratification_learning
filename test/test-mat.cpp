#include "linalg.h"
#include "except.h"

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

    ASSERT_EQ(MatrixEntry<ternary>({1,1}),   A(0,0));
    ASSERT_EQ(MatrixEntry<ternary>({-1,2}),  A(0,1));
    ASSERT_EQ(0,                             A(0,2));

    ASSERT_EQ(MatrixEntry<ternary>({-1,-1}), A(1,0));
    ASSERT_EQ(0,                             A(1,1));
    ASSERT_EQ(MatrixEntry<ternary>({1,1}),   A(1,2));

    ASSERT_EQ(0,                             A(2,0));
    ASSERT_EQ(MatrixEntry<ternary>({1,-2}),  A(2,1));
    ASSERT_EQ(MatrixEntry<ternary>({-1,-1}), A(2,2));

    // test if an exception is throws if an incorrect number of row or column times is specified
#ifndef NDEBUG
    EXPECT_DEATH(
        TernaryMatrix C(
            {
                { 2, 1, 0 },
                { 0, 2, 1 }
            },
            { 2, 1 },
            { 1, 3, 8 }
        );,
        ""
    );
#endif
#ifndef NDEBUG
    EXPECT_DEATH(
        TernaryMatrix C(
            {
                { 2, 1, 0 },
                { 0, 2, 1 }
            },
            { 2, 3 },
            { 1, 3, 2 }
        );,
        ""
    );
#endif
#ifndef NDEBUG
    EXPECT_DEATH(
        TernaryMatrix C(
            {
                { 2, 1, 0 },
                { 0, 2, 1 }
            },
            { 2, 4, 6 },
            { 1, 3, 8 }
        );,
        ""
    );
#endif
#ifndef NDEBUG
    EXPECT_DEATH(
        TernaryMatrix D(
            {
                { 2, 1, 0 },
                { 0, 2, 1 }
            },
            { 2, 4 },
            { 1, 3, 8, 10 }
        );,
        ""
    );
#endif
}


TEST(Matrix, equality) {
    TernaryMatrix A {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, 1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };
    TernaryMatrix B {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, -1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };
    TernaryMatrix C {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, 1, -1 }
        },
        { 0, 2, 3 },
        { 1, 2, 3 }
    };

    ASSERT_EQ(A, A);
    ASSERT_NE(A, B);
    ASSERT_NE(A, C);
    ASSERT_NE(B, C);

    ASSERT_EQ(A(1,1), B(1,1));
    ASSERT_EQ(A(1,1), C(1,1));
    ASSERT_NE(A(2,1), B(2,1));
    ASSERT_NE(A(2,1), C(2,1));
}

TEST(Matrix, copy) {
    TernaryMatrix A {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, 1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };
    TernaryMatrix B {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, -1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };

    TernaryMatrix A1 = A;
    TernaryMatrix A2 = A;

    ASSERT_NE(A, B);
    ASSERT_EQ(A, A1);
    ASSERT_EQ(A1, A2);
}

TEST(Matrix, move) {
    TernaryMatrix A {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, 1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };
    TernaryMatrix B {
        {
            { 1, -1, 0 },
            { -1, 0, 1 },
            { 0, -1, -1 }
        },
        { 0, 2, 4 },
        { 1, 2, 3 }
    };

    TernaryMatrix A1 = A;
    TernaryMatrix A2 = std::move(A);

    ASSERT_NE(A1, B);
    ASSERT_EQ(A1, A2);
    ASSERT_NE(A1, A);

    // test constructor
    TernaryMatrix A3 {std::move(A2)};

    ASSERT_NE(A1, A2);
    ASSERT_EQ(A1, A3);
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

TEST(Matrix, resize) {
    BinaryMatrix A = {
        {1, 1, 1, 1},
        {1, 1, 1, 1}
    };

    A.resize(6,1);

    ASSERT_EQ(6, A.rows());
    ASSERT_EQ(1, A.cols());


    // test with time (the times should be set to 0)
    BinaryMatrix B {
        {
            {1, 1, 1, 1},
            {1, 2, 1, 0}
        },
        { 1, 2 },
        { 3, 4, 5, 6 }
    };

    B.resize(3, 2);

    ASSERT_EQ(3, B.rows());
    ASSERT_EQ(2, B.cols());
    for (int rowN = 0; rowN < 3; rowN++) {
        for (int colN = 0; colN < 2; colN++) {
            ASSERT_EQ(0, B.getEntryTime(rowN, colN));
        }
    }
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

    TernaryMatrix A_time = {
        {
            { 0, 0, 0, 2, 0, 2, 0 },
            { 0, 0, 0, 1, 2, 0, 0 },
            { 0, 0, 0, 0, 1, 1, 0 },
            { 0, 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0, 2 },
            { 0, 0, 0, 0, 0, 0, 0 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 0, 1, 1, 2, 2, 2 }
    };
    TernaryMatrix B_time = {
        {
            { 1,    0, 0,    1, 0, 0,    0, 0,    0, 0 },
            { 0,    1, 0,    0, 0, 0,    0, 0,    0, 0 },

            { 0,    0, 1,    0, 0, 0,    0, 0,    0, 0 },
            { 0,    0, 0,    0, 1, 0,    1, 0,    0, 0 },

            { 0,    0, 0,    0, 0, 1,    0, 0,    0, 0 },
            { 0,    0, 0,    0, 0, 0,    0, 1,    1, 0 },
            { 0,    0, 0,    0, 0, 0,    0, 0,    0, 1 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 1, 1, 2, 2, 2, 3, 3, 4, 4 }
    };
    TernaryMatrix C_time = {
        {
            { 0,   0, 0,    0, 2, 0,    2, 2,    2, 0 },
            { 0,   0, 0,    0, 1, 2,    1, 0,    0, 0 },

            { 0,   0, 0,    0, 0, 1,    0, 1,    1, 0 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 1 },

            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 1 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 2 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 0 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 1, 1, 2, 2, 2, 3, 3, 4, 4 }
    };

    ASSERT_EQ(A_time * B_time, C_time);

    TernaryTimeVector AtimesB7 = TernaryTimeVector({ -1, 0, 1, 0, 0, 0, 0 }, { 0, 0, 1, 1, 2, 2, 2 }, 3);
    ASSERT_EQ(AtimesB7, A_time * B_time[7]);
}

TEST(Matrix, isEchelonForm) {
    TernaryMatrix pos1 = {
        { 2, 2, 0, 0, 1 },
        { 1, 0, 2, 2, 2 },
        { 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0 }
    };
    TernaryMatrix neg1 = {
        { 2, 2, 0, 0, 1 },
        { 1, 0, 2, 2, 2 },
        { 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 1 },
        { 0, 0, 0, 0, 0 }
    };

    ASSERT_TRUE(pos1.isEchelonForm());
    ASSERT_FALSE(neg1.isEchelonForm());

    TernaryMatrix pos2 = {
        { 2, 2, 0, 0, 1 },
        { 1, 0, 0, 2, 2 },
        { 0, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0 }
    };
    TernaryMatrix neg2 = {
        { 2, 2, 0, 0, 1 },
        { 1, 0, 0, 2, 2 },
        { 0, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 0 }
    };

    ASSERT_TRUE(pos2.isEchelonForm());
    ASSERT_FALSE(neg2.isEchelonForm());

    TernaryMatrix pos3 = {
        {
            { 2, 2, 0, 0, 1 },
            { 1, 0, 2, 2, 2 },
            { 0, 1, 1, 0, 0 },
            { 0, 0, 0, 1, 1 },
            { 0, 0, 0, 0, 0 }
        },
        { 0, 1, 2, 2, 3 },
        { 1, 2, 2, 3, 3 }
    };
    TernaryMatrix neg3 = {
        {
            { 2, 2, 0, 0, 1 },
            { 1, 0, 2, 2, 2 },
            { 0, 0, 1, 0, 0 },
            { 0, 1, 0, 1, 1 },
            { 0, 0, 0, 0, 0 }
        },
        { 0, 1, 2, 2, 3 },
        { 1, 2, 2, 3, 3 }
    };

    ASSERT_TRUE(pos3.isEchelonForm());
    ASSERT_FALSE(neg3.isEchelonForm());
}

TEST(Matrix, solve) {
    TernaryMatrix A_time = {
        {
            { 0, 0, 0, 2, 0, 2, 0 },
            { 0, 0, 0, 1, 2, 0, 0 },
            { 0, 0, 0, 0, 1, 1, 0 },
            { 0, 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0, 2 },
            { 0, 0, 0, 0, 0, 0, 0 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 0, 1, 1, 2, 2, 2 }
    };
    TernaryMatrix C_time = {
        {
            { 0,   0, 0,    0, 2, 0,    2, 2,    2, 0 },
            { 0,   0, 0,    0, 1, 2,    1, 0,    0, 0 },

            { 0,   0, 0,    0, 0, 1,    0, 1,    1, 0 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 1 },

            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 1 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 2 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 0 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 1, 1, 2, 2, 2, 3, 3, 4, 4 }
    };

    TernaryMatrix X;    solve(A_time, X, C_time);
    ASSERT_EQ(C_time, A_time * X);

    // reusing the same input
    solve(A_time, X, C_time);
    ASSERT_EQ(C_time, A_time * X);

#ifndef NDEBUG
    TernaryMatrix A_time1 = {
        {
            { 0, 0, 0, 0, 2, 2, 0 },
            { 0, 0, 0, 2, 1, 0, 0 },
            { 0, 0, 0, 1, 0, 1, 0 },
            { 0, 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0, 2 },
            { 0, 0, 0, 0, 0, 0, 0 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 0, 1, 1, 2, 2, 2 }
    };
    TernaryMatrix C_time1 = {
        {
            { 0,   0, 0,    0, 2, 0,    2, 2,    2, 0 },
            { 0,   0, 0,    0, 1, 2,    1, 0,    0, 0 },

            { 0,   0, 0,    0, 0, 1,    0, 1,    1, 0 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 1 },

            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 1 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 2 },
            { 0,   0, 0,    0, 0, 0,    0, 0,    0, 0 }
        },
        { 0, 0, 1, 1, 2, 2, 2 },
        { 0, 1, 1, 2, 2, 2, 3, 3, 4, 4 }
    };
    TernaryMatrix X1;
    EXPECT_DEATH({
            solve(A_time1, X1, C_time1);
        },
        ""
    );
#endif

    TernaryMatrix A2 = {
        {
            { 1, 0 },
            { 0, 0 },
            { 0, 1 }
        },
        { 0, 1, 2 },
        { 0, 2 }
    };
    TernaryMatrix B2 = {
        {
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 1, 1 }
        },
        { 0, 1, 2 },
        { 0, 2, 4 }
    };

    TernaryMatrix X2;
    ASSERT_THROW(solve(A2, X2, B2), except::NotInImageSpaceException);
}




// TODO add tests for IVector
//  - comparison
