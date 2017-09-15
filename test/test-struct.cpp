#include "linalg.h"
#include "toprep.h"

#include "gtest/gtest.h"

using namespace toprep;
using namespace la;

TEST(Map, apply) {
    TernaryMap map = {
        { 0, 1, 1, 1, 0, 1 },
        { 1, 0, 0, 0, 0, 1 },
        { 0, 2, 0, 1, 0, 0 },
        { 2, 2, 1, 2, 0, 0 }
    };

    TernarySpace space = {
        { 2, 0, 1, 0, 1, 1 },
        { 0, 1, 1, 2, 2, 2 },
        { 2, 2, 2, 2, 0, 2 },
        { 2, 1, 1, 2, 2, 2 },
        { 1, 1, 1, 0, 0, 2 },
        { 1, 0, 2, 0, 1, 1 }
    };

    TernarySpace expected_result = {
        { 2, 1, 0, 0, 2, 1 },
        { 0, 0, 0, 0, 2, 2 },
        { 2, 0, 0, 0, 0, 0 },
        { 1, 0, 2, 1, 1, 0 }
    };

    TernarySpace result = map(space);

    ASSERT_EQ(expected_result, result);
}

TEST(Map, decompose) {
    // TEST 1
    const int image_small_dim = 3;
    const int kernel_small_dim = 3;

    TernaryMap map_small {
        { 2, 2, 0, 0, 0, 0 },
        { 1, 0, 0, 2, 2, 0 },
        { 0, 1, 0, 1, 0, 2 },
        { 0, 0, 0, 0, 1, 1 }
    };

    TernarySpace kernel_small, image_small;
    map_small.decompose(kernel_small, image_small);

    ASSERT_EQ(image_small_dim, image_small.vector_count());
    ASSERT_EQ(kernel_small_dim, kernel_small.vector_count());

    for (int colN = 0; colN < kernel_small_dim; colN++) {
        const TernaryVectorWrapper kernel_vec = kernel_small[colN];
        ASSERT_TRUE(map_small(kernel_vec).isZero());
    }

    // TEST 2
    const int n_simplexes = 12;
    const int n_cycles = 7;

    const int kernel_big_dim = n_cycles;
    const int image_big_dim = n_simplexes - kernel_big_dim;

    // map_big represents a boundary map for triangles abc and acd with a lonely point e
    TernaryMap map_big = {
        { 0, 0, 0, 0, 0,    2, 2, 2, 0, 0,   0, 0 },
        { 0, 0, 0, 0, 0,    1, 0, 0, 2, 0,   0, 0 },
        { 0, 0, 0, 0, 0,    0, 1, 0, 1, 2,   0, 0 },
        { 0, 0, 0, 0, 0,    0, 0, 1, 0, 1,   0, 0 },
        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   0, 0 },

        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   1, 0 },
        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   2, 1 },
        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   0, 2 },
        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   1, 0 },
        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   0, 1 },

        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   0, 0 },
        { 0, 0, 0, 0, 0,    0, 0, 0, 0, 0,   0, 0 }
    };

    TernarySpace kernel_big, image_big;
    map_big.decompose(kernel_big, image_big);

    ASSERT_EQ(kernel_big_dim, kernel_big.vector_count());
    ASSERT_EQ(image_big_dim, image_big.vector_count());

    for (int colN = 0; colN < kernel_big_dim; colN++) {
        const TernaryVectorWrapper kernel_vec = kernel_big[colN];
        ASSERT_TRUE(map_big(kernel_vec).isZero());
    }
}

TEST(TimedMap, decompose) {
    std::vector<tstep> simplex_times = { 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4 };

    TernaryMap boundry {
        {
            { 0, 0,    0,-1,    0, 0,    -1,-1, 0,    0, 0 },
            { 0, 0,    0, 1,    0,-1,     0, 0, 0,    0, 0 },

            { 0, 0,    0, 0,    0, 1,     1, 0, 0,   -1, 0 },
            { 0, 0,    0, 0,    0, 0,     0, 0, 1,    0, 0 },

            { 0, 0,    0, 0,    0, 0,     0, 1, 0,    1, 0 },
            { 0, 0,    0, 0,    0, 0,     0, 0, 1,    0, 0 },

            { 0, 0,    0, 0,    0, 0,     0, 0,-1,    0, 1 },
            { 0, 0,    0, 0,    0, 0,     0, 0, 0,    0,-1 },
            { 0, 0,    0, 0,    0, 0,     0, 0, 0,    0, 0 },

            { 0, 0,    0, 0,    0, 0,     0, 0, 0,    0, 1 },
            { 0, 0,    0, 0,    0, 0,     0, 0, 0,    0, 0 }
        },
        simplex_times,
        simplex_times
    };

    TernarySpace expected_kernel = {
        {
            { 1, 0, 0, 0, 0, 0 },
            { 0, 1, 0, 0, 0, 0 },
            { 0, 0, 1, 0, 0, 0 },
            { 0, 0, 0, 0,-1, 1 },
            { 0, 0, 0, 1, 0, 0 },
            { 0, 0, 0, 0,-1, 1 },
            { 0, 0, 0, 0, 1, 0 },
            { 0, 0, 0, 0, 0,-1 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0, 0 }
        },
        simplex_times,
        { 0, 0, 1, 2, 3, 4 }
    };

    TernarySpace expected_image = {
        {
            {-1, 0,-1, 0, 0 },
            { 1,-1, 0, 0, 0 },
            { 0, 1, 0, 0, 0 },
            { 0, 0, 0, 1, 0 },
            { 0, 0, 1, 0, 0 },
            { 0, 0, 0, 1, 0 },
            { 0, 0, 0,-1, 1 },
            { 0, 0, 0, 0,-1 },
            { 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 1 },
            { 0, 0, 0, 0, 0 }
        },
        simplex_times,
        { 1, 2, 3, 3, 4 }
    };

    TernarySpace kernel, image;
    boundry.decompose(kernel, image);

    ASSERT_EQ(expected_kernel, kernel);
    ASSERT_EQ(expected_image, image);

    for (int colN = 0; colN < expected_kernel.cols(); colN++) {
        const TernaryVectorWrapper kernel_vec = kernel[colN];
        ASSERT_TRUE(boundry(kernel_vec).isZero());
    }
}
