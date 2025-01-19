// StringUtils_test.cpp
#include "gtest/gtest.h"
#include "StringUtils.h"
#include "Logger.h"

TEST( StringUtilsTest, ToUpperCase )
{
    EXPECT_EQ( StringUtils::toUpperCase( "hello" ), "HELLO" );
    EXPECT_EQ( StringUtils::toUpperCase( "World" ), "WORLD" );
}

TEST( StringUtilsTest, ToLowerCase )
{
    EXPECT_EQ( StringUtils::toLowerCase( "HELLO" ), "hello" );
    EXPECT_EQ( StringUtils::toLowerCase( "World" ), "world" );
}

TEST( StringUtilsTest, StartsWith )
{
    EXPECT_TRUE( StringUtils::startsWith( "hello", "he" ) );
    EXPECT_FALSE( StringUtils::startsWith( "hello", "lo" ) );
}

TEST( StringUtilsTest, EndsWith )
{
    EXPECT_TRUE( StringUtils::endsWith( "hello", "lo" ) );
    EXPECT_FALSE( StringUtils::endsWith( "hello", "he" ) );
}

TEST( StringUtilsTest, Trim )
{
    EXPECT_EQ( StringUtils::trim( "   hello   " ), "hello" );
    EXPECT_EQ( StringUtils::trim( "  world" ), "world" );
    EXPECT_EQ( StringUtils::trim( "test  " ), "test" );
}

TEST( StringUtilsTest, Replace )
{
    EXPECT_EQ( StringUtils::replace( "hello world", "world", "there" ), "hello there" );
    EXPECT_EQ( StringUtils::replace( "foo bar foo", "foo", "baz" ), "baz bar baz" );
}

TEST( StringUtilsTest, Search )
{
    auto results = StringUtils::search( "hello world, hello universe", "hello" );
    EXPECT_EQ( results.size(), 2 );
    EXPECT_EQ( results[0], "hello" );
    EXPECT_EQ( results[1], "hello" );
}

TEST( StringUtilsTest, Remove )
{
    EXPECT_EQ( StringUtils::remove( "foo bar foo", "foo" ), " bar " );
    EXPECT_EQ( StringUtils::remove( "hello world, hello universe", "hello " ), "world, universe" );
}

int main( int argc, char **argv )
{
    Logger::SetPriority( LogLevel::TRACE );
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
