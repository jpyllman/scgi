#include "libscgi.hpp"

#include <regex>

namespace scgi {

static std::regex scgi_begin_re( R"((\d+):CONTENT_LENGTH)" );

std::size_t request_map::parse_scgi_header( std::byte const *scgi, std::size_t sz )
{
  std::size_t pos = 0;

  if( scgi != nullptr )
  {
    char const *k_pos = reinterpret_cast<char const *>( scgi );

    while( ( pos < sz ) )
    {
      std::string_view k( k_pos );
      char const *v_pos = k_pos + k.length() + 1;
      std::string_view v( v_pos );

      // check if key exists??
      index.emplace_back( k, v );

      k_pos = v_pos + v.length() + 1;
      pos += k.length() + v.length() + 2;
    }

    // check if next byte is a ',' character
  }

  // check if key exist multiple times??
  return pos;
}

std::tuple<std::size_t, std::size_t> check_scgi( std::byte const *scgi )
{
  std::size_t bytes_in_head = 0;
  std::size_t size_of_start = 0;

  std::cmatch m;
  if( std::regex_match( reinterpret_cast<char const *>( scgi ), m, scgi_begin_re ) )
  {
    bytes_in_head = static_cast<std::size_t>( std::atol( m[1].first ) );
    size_of_start = static_cast<std::size_t>( m[1].length() + 1u );
  }

  // check if next byte is a ',' character
  
  return std::tuple<int, int>{bytes_in_head, size_of_start};
}

} // namespace scgi
