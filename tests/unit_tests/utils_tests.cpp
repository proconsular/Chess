//
// Created by Chris Luttio on 1/9/22.
//

#include "gtest/gtest.h"
#include "utils.h"

#include <vector>

using namespace std;

TEST(utils_tests, zip) {
    {
        vector<int> a{1, 2, 3, 4};
        vector<int> b{10, 11, 12, 13};

        vector<int> spec{1, 10, 2, 11, 3, 12, 4, 13};

        EXPECT_EQ(spec, zip(a, b));
    }
    {
        vector<int> a{1, 2, 3, 4};
        vector<int> b{10, 11, 12, 13, 14};

        vector<int> spec{1, 10, 2, 11, 3, 12, 4, 13, 14};

        EXPECT_EQ(spec, zip(a, b));
    }
    {
        vector<int> a{1, 2, 3, 4, 5};
        vector<int> b{10, 11, 12, 13};

        vector<int> spec{1, 10, 2, 11, 3, 12, 4, 13, 5};

        EXPECT_EQ(spec, zip(a, b));
    }
}

TEST(utils_tests, zip_with_pieces) {

}