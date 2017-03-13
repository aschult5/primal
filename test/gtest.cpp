#include <gtest/gtest.h>

#include <cstdint>

#include "miller_rabin.hpp"


//
//MillerRabin
//
TEST(MillerRabinTest, 32bitPrime)
{
   EXPECT_EQ(true, custom::miller_rabin_test(static_cast<uint32_t>(4294293967u)));
}
TEST(MillerRabinTest, 32bitComposite)
{
   EXPECT_EQ(false, custom::miller_rabin_test(static_cast<uint32_t>(4294293968u)));
}
TEST(MillerRabinTest, 64bitPrime)
{
   EXPECT_EQ(true, custom::miller_rabin_test(static_cast<uint64_t>(18446744073709551557u)));
}
TEST(MillerRabinTest, 64bitComposite)
{
   EXPECT_EQ(false, custom::miller_rabin_test(static_cast<uint64_t>(18446744073709551558u)));
}

int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}