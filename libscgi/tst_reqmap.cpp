#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "libscgi.hpp"

TEST_CASE( "minimal SCGI header", "[basic]" )
{
  // the value 28 is very sensetive to the string **WARNING**
  std::vector<std::byte> in_data{28};
  const char *raw_data = "24:CONTENT_LENGTH\0000\000SCGI\0001\000,";
  for( int i = 0; i < 28; ++i ) in_data[i] = static_cast<std::byte>( raw_data[i] );

  REQUIRE( in_data.size() == 28 );

  auto [header_size, start_size] = scgi::check_scgi( in_data.data() );
  REQUIRE( start_size == 3 );
  REQUIRE( header_size == 24 );
  REQUIRE( in_data[( start_size + header_size )] == (std::byte)',' ); // next 'char' should be a ','

  scgi::request_map rmap;
  auto read_size = rmap.parse_scgi_header( in_data.data() + start_size, header_size );
  REQUIRE( read_size == header_size );

  REQUIRE( rmap.at( "SCGI") == "1" );
  REQUIRE( rmap.at( "CONTENT_LENGTH" ) == "0" );
}
