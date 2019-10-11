#include "libscgi.hpp"

#include <regex>

namespace scgi {

static std::regex netstring_begin_re( R"((\d+):.*)" );
static std::regex scgi_begin_re( R"((\d+):CONTENT_LENGTH)" );

std::size_t request_header::parse_scgi_header()
{
  std::size_t pos = 0;

  if( raw_header_size_ > 0 )
  {
    if( !index.empty() )
      index.clear();

    if( start_size == 0 )
      check_scgi();

    if( start_size > 0 )
    {
      char const *k_pos = reinterpret_cast<char const *>( raw_header_ );
      k_pos += start_size;

      while( pos < header_size )
      {
        std::string_view k( k_pos );
        char const *v_pos = k_pos + k.length() + 1;
        std::string_view v( v_pos );

        // check if key exists??

        index.emplace( std::lower_bound( index.begin(), index.end(), k,
                           []( index_data_type const &a, key_type const &b ) -> bool { return a.first.compare( b ) < 0; } ),
            k, v );

        k_pos = v_pos + v.length() + 1;
        pos += k.length() + v.length() + 2;
      }

      // check if pos == raw_
      // check if next byte is a ',' character
    }
  }

  // check if key exist multiple times??

  return pos;
}

std::tuple<std::size_t, std::size_t> check_netstring( std::byte const *scgi, bool only_check_beginning )
{
  std::size_t bytes_in_head = 0;
  std::size_t size_of_start = 0;

  std::cmatch m;
  if( std::regex_match( reinterpret_cast<char const *>( scgi ), m, netstring_begin_re ) )
  {
    bytes_in_head = static_cast<std::size_t>( std::atol( m[1].first ) );
    size_of_start = static_cast<std::size_t>( m[1].length() + 1u );
  }

  // The last character of the header should be a ','
  if( !only_check_beginning && ( size_of_start > 0 ) )
  {
    if( scgi[size_of_start + bytes_in_head] != static_cast<std::byte>( ',' ) )
    {
      size_of_start = bytes_in_head = 0;
    }
  }

  return std::tuple<int, int>{bytes_in_head, size_of_start};
}

std::tuple<std::size_t, std::size_t> check_scgi( std::byte const *scgi, bool only_check_beginning )
{
  std::size_t bytes_in_head = 0;
  std::size_t size_of_start = 0;

  std::cmatch m;
  if( std::regex_match( reinterpret_cast<char const *>( scgi ), m, scgi_begin_re ) )
  {
    bytes_in_head = static_cast<std::size_t>( std::atol( m[1].first ) );
    size_of_start = static_cast<std::size_t>( m[1].length() + 1u );
  }

  // The last character of the header should be a ','
  if( !only_check_beginning && ( size_of_start > 0 ) )
  {
    if( scgi[size_of_start + bytes_in_head] != static_cast<std::byte>( ',' ) )
    {
      size_of_start = bytes_in_head = 0;
    }
  }

  return std::tuple<int, int>{bytes_in_head, size_of_start};
}

std::unordered_map<std::string, std::string> parse_scgi_header( std::byte const *scgi, std::size_t sz )
{
  std::unordered_map<std::string, std::string> req;

  if( scgi != nullptr )
  {
    std::size_t pos = 0;
    char const *k_pos = reinterpret_cast<char const *>( scgi );

    while( ( pos < sz ) )
    {
      std::size_t k_len = std::strlen( k_pos );
      char const *v_pos = k_pos + k_len + 1;
      std::size_t v_len = std::strlen( v_pos );

      req.emplace( std::string_view( k_pos, k_len ), std::string_view( v_pos, v_len ) );

      k_pos = v_pos + v_len + 1;
      pos += k_len + v_len + 2;
    }
  }

  return req;
}

} // namespace scgi
