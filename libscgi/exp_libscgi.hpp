#if !defined( SCGI_EXP_LIBSCGI_HPP )
#define SCGI_EXP_LIBSCGI_HPP

#include "libscgi.hpp"

#include <string>

namespace scgi { namespace exp {

namespace string_map {
// use strings and unordered_map for the index
class request_header
{
  typedef typename std::string key_type;
  typedef typename std::string value_type;
  typedef typename std::unordered_map<key_type, value_type> index_type;

public:
  request_header() { index.reserve( 48 ); }

  std::size_t capacity() const { return index.max_size(); /*index.capacity();*/ }
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

  std::optional<value_type> const at( key_type const &key ) const
  {
    for( auto const &[k, v] : index )
    {
      if( k.compare( key ) == 0 )
        return std::make_optional<value_type>( v );;
    }

    return std::optional<value_type> {};
  }

  std::size_t parse_scgi_header( std::byte const *, std::size_t );

private:
  index_type index;
  // std::array<std::pair<std::string_view, std::string_view>, 48> index;
};

} // namespace string_map

namespace liniar_search {
// use find_if() or simular, no sorted index, string_view
class request_header
{
  typedef typename std::string_view key_type;
  typedef typename std::string_view value_type;
  typedef typename std::pair<key_type, value_type> index_data_type;
  typedef typename std::vector<index_data_type> index_type;

public:
  request_header() { index.reserve( 48 ); }

  std::size_t capacity() const { return index.capacity(); }
  std::size_t size() const { return index.size(); }

  bool contains( key_type const &key ) const
  {
    // will this make it slow?
    if( index.empty() )
      return false;

    auto itr = std::find_if(
        index.begin(), index.end(), [&key]( index_data_type const &a ) -> bool { return a.first.compare( key ) == 0; } );

    return itr != index.end();
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
};

} // namespace liniar_search

namespace binary_search {
// use lower_bounds() to find in index needs a sorted index
// sort by inserting in sorded order, not sort at the end, string_view
class request_header
{};

} // namespace binary_search

}} // namespace scgi::exp

#endif // !defined( SCGI_EXP_LIBSCGI_HPP )
