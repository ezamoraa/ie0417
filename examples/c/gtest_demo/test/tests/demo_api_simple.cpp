#include "gtest/gtest.h"

#include <demo_api.h>

/** Test the basic addition operation from demo_api_add */
TEST(demo_api_simple, add_op)
{
    int ret = 0;
    int out = 0;

    ret = demo_api_add(5, 9, &out);
    ASSERT_EQ(ret, DEMO_API_OK);
    ASSERT_EQ(out, 14);
}

/** Test the sanity checks from demo_api_add */
TEST(demo_api_simple, add_sanity)
{
    int ret = 0;

    // Pass NULL out param, expect PARM error
    ret = demo_api_add(5, 9, NULL);
    ASSERT_EQ(ret, DEMO_API_PARM);
}

/** Test the basic multiplication operation from demo_api_mult */
TEST(demo_api_simple, mult_op)
{
    int ret = 0;
    int out = 0;

    ret = demo_api_mult(5, 4, &out);
    ASSERT_EQ(ret, DEMO_API_OK);
    ASSERT_EQ(out, 20);
}

/** Test the sanity checks from demo_api_mult */
TEST(demo_api_simple, mult_sanity)
{
    int ret = 0;

    // Pass NULL out param, expect PARM error
    ret = demo_api_mult(5, 9, NULL);
    ASSERT_EQ(ret, DEMO_API_PARM);
}

/** Test add and multiply demo APIs with EXPECT checks */
TEST(demo_api_simple, add_mult_expect)
{
    int ret = 0;
    int out = 0;

    ret = demo_api_add(5, 9, &out);
    ASSERT_EQ(ret, DEMO_API_OK);
    // EXPECT_EQ(out, 17); // -> uncomment for failure
    /* NOTE: The test should continue even if EXPECT_EQ fails */
    std::cout << "Now proceed to testing mult demo API!"<< std::endl;

    ret = demo_api_mult(5, 4, &out);
    ASSERT_EQ(ret, DEMO_API_OK);
    EXPECT_EQ(out, 20);
}

/** Test double positive operation from demo_api_double_if_pos */
TEST(demo_api_simple, double_if_pos_op)
{
    int ret = 0;
    int out = 0;

    ret = demo_api_double_if_pos(5, &out);
    ASSERT_EQ(ret, DEMO_API_OK);
    EXPECT_EQ(out, 10);
}

/** Test the sanity checks from demo_api_double_if_pos */
TEST(demo_api_simple, double_if_pos_sanity)
{
    int ret = 0;
    int out = 0;

    // Pass NULL out param, expect PARM error
    ret = demo_api_double_if_pos(10, NULL);
    ASSERT_EQ(ret, DEMO_API_PARM);

    // Pass negative input, expect RANGE error
    ret = demo_api_double_if_pos(-4, &out);
    ASSERT_EQ(ret, DEMO_API_RANGE);
}
