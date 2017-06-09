#include "struct.h"

#include "gtest/gtest.h"

using namespace strct;

TEST(Module, constructor) {
    BinaryMap boundry = {
        {
            { 0, 0,    0, 0, 1, 0,    1, 0,    1,    0, 0 },
            { 0, 0,    0, 0, 1, 1,    0, 0,    0,    0, 0 },

            { 0, 0,    0, 0, 0, 1,    0, 1,    1,    0, 0 },
            { 0, 0,    0, 0, 0, 0,    1, 1,    0,    0, 0 },
            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    1, 0 },
            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    1, 0 },

            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    0, 1 },
            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    0, 1 },

            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    1, 1 },

            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    0, 0 },
            { 0, 0,    0, 0, 0, 0,    0, 0,    0,    0, 0 },
        },
        { 0, 0, 1, 1, 1, 1, 2, 2, 3, 4, 5 },
        { 0, 0, 1, 1, 1, 1, 2, 2, 3, 4, 5 }
    };
    std::vector<std::pair<tstep,tstep>> barcode;

    BinaryModule module {boundry};
    module.getBarcode(barcode);

    auto contains = [&](const std::pair<tstep,tstep>& interval) { return std::find(barcode.begin(), barcode.end(), interval) != barcode.end(); };

    ASSERT_EQ(6, barcode.size());
    ASSERT_TRUE(contains({ 0, tstep::INF }));
    ASSERT_TRUE(contains({ 0, 1 }));
    ASSERT_TRUE(contains({ 1, 1 }));
    ASSERT_TRUE(contains({ 1, 2 }));
    ASSERT_TRUE(contains({ 2, 5 }));
    ASSERT_TRUE(contains({ 3, 4 }));
}
