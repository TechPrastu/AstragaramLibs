#include <gtest/gtest.h>
#include "person.pb.h"
#include <google/protobuf/util/json_util.h>

TEST( ProtobufTest, ProtoToJson )
{
    // Create a Person message
    Person person;
    person.set_name( "Alice" );
    person.set_id( 123 );
    person.set_email( "alice@example.com" );

    // Convert Proto to JSON
    std::string json_str;
    google::protobuf::util::MessageToJsonString( person, &json_str );
    std::string expected_json = R"({"name":"Alice","id":123,"email":"alice@example.com"})";

    EXPECT_EQ( json_str, expected_json );
}

TEST( ProtobufTest, JsonToProto )
{
    // JSON string
    std::string json_str = R"({"name":"Alice","id":123,"email":"alice@example.com"})";
    // Convert JSON to Proto
    Person person;
    auto status = google::protobuf::util::JsonStringToMessage( json_str, &person );

    EXPECT_TRUE( status.ok() );
    EXPECT_EQ( person.name(), "Alice" );
    EXPECT_EQ( person.id(), 123 );
    EXPECT_EQ( person.email(), "alice@example.com" );
}

TEST( ProtobufTest, ProtoToString )
{
    // Create a Person message
    Person person;
    person.set_name( "Alice" );
    person.set_id( 123 );
    person.set_email( "alice@example.com" );

    // Convert Proto to string
    std::string proto_str;
    person.SerializeToString( &proto_str );

    // Deserialize string to Proto
    Person new_person;
    new_person.ParseFromString( proto_str );

    EXPECT_EQ( new_person.name(), "Alice" );
    EXPECT_EQ( new_person.id(), 123 );
    EXPECT_EQ( new_person.email(), "alice@example.com" );
}

TEST( ProtobufTest, StringToProto )
{
    // Create a Person message
    Person person;
    person.set_name( "Alice" );
    person.set_id( 123 );
    person.set_email( "alice@example.com" );

    // Convert Proto to string
    std::string proto_str;
    person.SerializeToString( &proto_str );

    // Deserialize string to Proto
    Person new_person;
    bool success = new_person.ParseFromString( proto_str );

    EXPECT_TRUE( success );
    EXPECT_EQ( new_person.name(), "Alice" );
    EXPECT_EQ( new_person.id(), 123 );
    EXPECT_EQ( new_person.email(), "alice@example.com" );
}

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
