#include <vector>
//#include <array>
#include <string_view>

namespace scgi {

class request_map
{
  typedef typename std::string_view key_type;
  typedef typename std::string_view value_type;
  typedef typename std::vector<std::pair<key_type, value_type>> index_type;

public:
  request_map() { index.reserve( 48 ); }

  std::size_t capacity() const { return index.capacity(); }
  std::size_t size() const { return index.size(); }

  bool contains( key_type const &key ) const
  {
    for( auto const &[k, v] : index )
    {
      if( k.compare( key ) == 0 )
        return true;
    }

    return false;
  }

  value_type const &at( key_type const &key ) const
  {
    for( auto const &[k, v] : index )
    {
      if( k.compare( key ) == 0 )
        return v;
    }

    throw std::out_of_range( "not found" );
  }

  std::size_t parse_scgi_header( std::byte const *, std::size_t );

private:
  index_type index;
  // std::array<std::pair<std::string_view, std::string_view>, 48> index;
};

std::tuple<std::size_t, std::size_t> check_scgi( std::byte const * );

} // namespace scgi
