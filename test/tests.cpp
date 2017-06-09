#include "gtest/gtest.h"

#include "test-num.cpp"
#include "test-mat.cpp"
#include "test-vec.cpp"
#include "test-struct.cpp"
#include "test-module.cpp"
#include "test-simplex.cpp"
#include "test-complex.cpp"
#include "test-triangulation.cpp"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}
