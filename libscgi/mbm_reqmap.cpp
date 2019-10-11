#include <celero/Celero.h>

#include "libscgi.hpp"

#include <iostream>

CELERO_MAIN

std::size_t get_data( std::byte *data )
{
  static const unsigned char raw_data[] = {0x37, 0x36, 0x31, 0x3a, 0x43, 0x4f, 0x4e, 0x54, 0x45, 0x4e, 0x54, 0x5f, 0x4c, 0x45, 0x4e,
      0x47, 0x54, 0x48, 0x00, 0x30, 0x00, 0x53, 0x43, 0x47, 0x49, 0x00, 0x31, 0x00, 0x70, 0x72, 0x6f, 0x78, 0x79, 0x2d, 0x73, 0x63,
      0x67, 0x69, 0x2d, 0x70, 0x61, 0x74, 0x68, 0x69, 0x6e, 0x66, 0x6f, 0x00, 0x31, 0x00, 0x48, 0x54, 0x54, 0x50, 0x5f, 0x48, 0x4f,
      0x53, 0x54, 0x00, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x35, 0x36, 0x2e, 0x31, 0x36, 0x3a, 0x38, 0x30, 0x31, 0x30,
      0x00, 0x48, 0x54, 0x54, 0x50, 0x5f, 0x55, 0x53, 0x45, 0x52, 0x5f, 0x41, 0x47, 0x45, 0x4e, 0x54, 0x00, 0x63, 0x75, 0x72, 0x6c,
      0x2f, 0x37, 0x2e, 0x36, 0x35, 0x2e, 0x30, 0x00, 0x48, 0x54, 0x54, 0x50, 0x5f, 0x41, 0x43, 0x43, 0x45, 0x50, 0x54, 0x00, 0x2a,
      0x2f, 0x2a, 0x00, 0x50, 0x41, 0x54, 0x48, 0x00, 0x2f, 0x75, 0x73, 0x72, 0x2f, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x2f, 0x73, 0x62,
      0x69, 0x6e, 0x3a, 0x2f, 0x75, 0x73, 0x72, 0x2f, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x2f, 0x62, 0x69, 0x6e, 0x3a, 0x2f, 0x75, 0x73,
      0x72, 0x2f, 0x73, 0x62, 0x69, 0x6e, 0x3a, 0x2f, 0x75, 0x73, 0x72, 0x2f, 0x62, 0x69, 0x6e, 0x3a, 0x2f, 0x73, 0x62, 0x69, 0x6e,
      0x3a, 0x2f, 0x62, 0x69, 0x6e, 0x00, 0x53, 0x45, 0x52, 0x56, 0x45, 0x52, 0x5f, 0x53, 0x49, 0x47, 0x4e, 0x41, 0x54, 0x55, 0x52,
      0x45, 0x00, 0x3c, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x3e, 0x41, 0x70, 0x61, 0x63, 0x68, 0x65, 0x2f, 0x32, 0x2e, 0x34,
      0x2e, 0x33, 0x38, 0x20, 0x28, 0x44, 0x65, 0x62, 0x69, 0x61, 0x6e, 0x29, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x61,
      0x74, 0x20, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x35, 0x36, 0x2e, 0x31, 0x36, 0x20, 0x50, 0x6f, 0x72, 0x74, 0x20,
      0x38, 0x30, 0x31, 0x30, 0x3c, 0x2f, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x3e, 0x0a, 0x00, 0x53, 0x45, 0x52, 0x56, 0x45,
      0x52, 0x5f, 0x53, 0x4f, 0x46, 0x54, 0x57, 0x41, 0x52, 0x45, 0x00, 0x41, 0x70, 0x61, 0x63, 0x68, 0x65, 0x2f, 0x32, 0x2e, 0x34,
      0x2e, 0x33, 0x38, 0x20, 0x28, 0x44, 0x65, 0x62, 0x69, 0x61, 0x6e, 0x29, 0x00, 0x53, 0x45, 0x52, 0x56, 0x45, 0x52, 0x5f, 0x4e,
      0x41, 0x4d, 0x45, 0x00, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x35, 0x36, 0x2e, 0x31, 0x36, 0x00, 0x53, 0x45, 0x52,
      0x56, 0x45, 0x52, 0x5f, 0x41, 0x44, 0x44, 0x52, 0x00, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x35, 0x36, 0x2e, 0x31,
      0x36, 0x00, 0x53, 0x45, 0x52, 0x56, 0x45, 0x52, 0x5f, 0x50, 0x4f, 0x52, 0x54, 0x00, 0x38, 0x30, 0x31, 0x30, 0x00, 0x52, 0x45,
      0x4d, 0x4f, 0x54, 0x45, 0x5f, 0x41, 0x44, 0x44, 0x52, 0x00, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x35, 0x36, 0x2e,
      0x31, 0x00, 0x44, 0x4f, 0x43, 0x55, 0x4d, 0x45, 0x4e, 0x54, 0x5f, 0x52, 0x4f, 0x4f, 0x54, 0x00, 0x2f, 0x76, 0x61, 0x72, 0x2f,
      0x77, 0x77, 0x77, 0x2f, 0x6a, 0x70, 0x79, 0x77, 0x70, 0x2d, 0x74, 0x73, 0x74, 0x00, 0x52, 0x45, 0x51, 0x55, 0x45, 0x53, 0x54,
      0x5f, 0x53, 0x43, 0x48, 0x45, 0x4d, 0x45, 0x00, 0x68, 0x74, 0x74, 0x70, 0x00, 0x43, 0x4f, 0x4e, 0x54, 0x45, 0x58, 0x54, 0x5f,
      0x50, 0x52, 0x45, 0x46, 0x49, 0x58, 0x00, 0x00, 0x43, 0x4f, 0x4e, 0x54, 0x45, 0x58, 0x54, 0x5f, 0x44, 0x4f, 0x43, 0x55, 0x4d,
      0x45, 0x4e, 0x54, 0x5f, 0x52, 0x4f, 0x4f, 0x54, 0x00, 0x2f, 0x76, 0x61, 0x72, 0x2f, 0x77, 0x77, 0x77, 0x2f, 0x6a, 0x70, 0x79,
      0x77, 0x70, 0x2d, 0x74, 0x73, 0x74, 0x00, 0x53, 0x45, 0x52, 0x56, 0x45, 0x52, 0x5f, 0x41, 0x44, 0x4d, 0x49, 0x4e, 0x00, 0x77,
      0x65, 0x62, 0x6d, 0x61, 0x73, 0x74, 0x65, 0x72, 0x40, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x00, 0x53, 0x43,
      0x52, 0x49, 0x50, 0x54, 0x5f, 0x46, 0x49, 0x4c, 0x45, 0x4e, 0x41, 0x4d, 0x45, 0x00, 0x70, 0x72, 0x6f, 0x78, 0x79, 0x3a, 0x73,
      0x63, 0x67, 0x69, 0x3a, 0x2f, 0x2f, 0x31, 0x32, 0x37, 0x2e, 0x30, 0x2e, 0x30, 0x2e, 0x31, 0x3a, 0x38, 0x30, 0x30, 0x30, 0x2f,
      0x00, 0x52, 0x45, 0x4d, 0x4f, 0x54, 0x45, 0x5f, 0x50, 0x4f, 0x52, 0x54, 0x00, 0x33, 0x37, 0x39, 0x34, 0x34, 0x00, 0x53, 0x45,
      0x52, 0x56, 0x45, 0x52, 0x5f, 0x50, 0x52, 0x4f, 0x54, 0x4f, 0x43, 0x4f, 0x4c, 0x00, 0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e,
      0x31, 0x00, 0x52, 0x45, 0x51, 0x55, 0x45, 0x53, 0x54, 0x5f, 0x4d, 0x45, 0x54, 0x48, 0x4f, 0x44, 0x00, 0x47, 0x45, 0x54, 0x00,
      0x51, 0x55, 0x45, 0x52, 0x59, 0x5f, 0x53, 0x54, 0x52, 0x49, 0x4e, 0x47, 0x00, 0x00, 0x52, 0x45, 0x51, 0x55, 0x45, 0x53, 0x54,
      0x5f, 0x55, 0x52, 0x49, 0x00, 0x2f, 0x00, 0x53, 0x43, 0x52, 0x49, 0x50, 0x54, 0x5f, 0x4e, 0x41, 0x4d, 0x45, 0x00, 0x00, 0x50,
      0x41, 0x54, 0x48, 0x5f, 0x49, 0x4e, 0x46, 0x4f, 0x00, 0x2f, 0x00, 0x50, 0x41, 0x54, 0x48, 0x5f, 0x54, 0x52, 0x41, 0x4e, 0x53,
      0x4c, 0x41, 0x54, 0x45, 0x44, 0x00, 0x70, 0x72, 0x6f, 0x78, 0x79, 0x3a, 0x73, 0x63, 0x67, 0x69, 0x3a, 0x2f, 0x2f, 0x31, 0x32,
      0x37, 0x2e, 0x30, 0x2e, 0x30, 0x2e, 0x31, 0x3a, 0x38, 0x30, 0x30, 0x30, 0x2f, 0x2f, 0x00, 0x2c};
  static constexpr std::size_t raw_size = sizeof( raw_data ) / sizeof( unsigned char );
  static_assert( raw_size == 766 );
  for( int i = 0; i < raw_size; ++i ) { data[i] = static_cast<std::byte>( raw_data[i] ); }

  return raw_size;
}

class Req1Fixture : public celero::TestFixture
{
public:
  virtual void setUp( const celero::TestFixture::ExperimentValue &x )
  {
    start_size = header_size = 0;
    req_map.clear();

    get_data( req_data );
    std::tie( header_size, start_size ) = scgi::check_scgi( req_data );

    req_map = scgi::parse_scgi_header( req_data + start_size, header_size );
  }

  virtual void tearDown() {}

public:
  std::size_t start_size = 0;
  std::size_t header_size = 0;

  std::byte req_data[1280];
  std::unordered_map<std::string, std::string> req_map;
};

class Req2Fixture : public celero::TestFixture
{
public:
  virtual void setUp( const celero::TestFixture::ExperimentValue &x )
  {
    req.clear();

    req.raw_header_size( get_data( req.raw_header() ) );
    req.check_scgi();

    req.parse_scgi_header();
  }

  virtual void tearDown() {}

public:
  scgi::request_header req;
};

BASELINE_F( ParseRequest, StringMap, Req1Fixture, 10, 1'000'000 )
{
  req_map.clear();
  req_map = scgi::parse_scgi_header( req_data + start_size, header_size );
}

BENCHMARK_F( ParseRequest, Optimized, Req2Fixture, 10, 1'000'000 ) { req.parse_scgi_header(); }

BASELINE_F( LookupKeyValue, StringMap, Req1Fixture, 10, 50'000'000 ) { auto x = req_map.at( "SCGI" ); }

BENCHMARK_F( LookupKeyValue, Optimized, Req2Fixture, 10, 50'000'000 ) { auto x = req.at( "SCGI" ); }