#include <gtest/gtest.h>

#include <cstdint>

#include "miller_rabin.hpp"
#include "primal.hpp"



TEST(MillerRabin, 32bitPrime)
{
   uint32_t n{4294293967u};

   ASSERT_EQ(true, custom::miller_rabin_test(n));
}

TEST(MillerRabin, 32bitComposite)
{
   uint64_t n{4294293968u};

   ASSERT_EQ(false, custom::miller_rabin_test(n));
}

TEST(MillerRabin, 64bitPrime)
{
   uint64_t n{18446744073709551557u};

   ASSERT_EQ(true, custom::miller_rabin_test(n));
}

TEST(MillerRabin, 64bitComposite)
{
   uint64_t n{18446744073709551558u};

   ASSERT_EQ(false, custom::miller_rabin_test(n));
}

int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
