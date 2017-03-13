#include <gtest/gtest.h>

#include <cstdint>

#include "miller_rabin.hpp"


//
//MillerRabin
//
TEST(MillerRabinTest, 32bitPrime)
{
   EXPECT_EQ(custom::primality::GUARANTEED, custom::primality_test(uint32_t{4294293967u}));
}
TEST(MillerRabinTest, 32bitComposite)
{
   EXPECT_EQ(custom::primality::NOT, custom::primality_test(uint32_t{4294293968u}));
}
TEST(MillerRabinTest, 64bitPrime)
{
   EXPECT_EQ(custom::primality::PROBABLY, custom::primality_test(uint64_t{18446744073709551557u}));
}
TEST(MillerRabinTest, 64bitComposite)
{
   EXPECT_EQ(custom::primality::NOT, custom::primality_test(uint64_t{18446744073709551558u}));
}

int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
