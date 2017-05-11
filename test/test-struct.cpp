#include "linalg.h"
#include "struct.h"

#include "gtest/gtest.h"

using namespace strct;
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
        const TernaryVector& kernel_vec = kernel_small[colN];
        ASSERT_TRUE(map_small(kernel_vec).is_zero());
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
        const TernaryVector& kernel_vec = kernel_big[colN];
        ASSERT_TRUE(map_big(kernel_vec).is_zero());
    }
}
