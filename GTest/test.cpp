#include "sum.h"
#include "gtest/gtest.h"

namespace
{
TEST( SumTest1, BasicSum )
{
    EXPECT_EQ( 4, sum( 1, 3 ) );
    EXPECT_EQ( 0, sum( 0, 0 ) );
}

TEST( SumTest2, BasicSum )
{
    EXPECT_EQ( 5, sum( -1, 6 ) );
    EXPECT_EQ( 3, sum( 4, -1 ) );
}
}


int main( int argc, char** argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}