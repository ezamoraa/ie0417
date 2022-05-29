#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
    int init_seed = 0;
    ::testing::InitGoogleTest(&argc, argv);

    /* NOTE: We will use cstdlib rand() to get the seeds for the
     * C++ pseudo-random number generators (RNGs). This sets the
     * initial seed with value from --gtest_random_seed via srand(),
     * to get reproducible seed values with rand() */
    init_seed = ::testing::GTEST_FLAG(random_seed);
    if (init_seed) srand(init_seed);

    return RUN_ALL_TESTS();
}
