#include "exp_libscgi.hpp"

#include <regex>

namespace scgi { namespace exp {

namespace string_map {
std::size_t request_header::parse_scgi_header( std::byte const *scgi, std::size_t sz )
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
      index.emplace( k, v );

      k_pos = v_pos + v.length() + 1;
      pos += k.length() + v.length() + 2;
    }

    // check if next byte is a ',' character
  }

  // check if key exist multiple times??
  return pos;
}

} // namespace string_map

namespace liniar_search {
}

namespace binary_search {
}

}} // namespace scgi::exp
