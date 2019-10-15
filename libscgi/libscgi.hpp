#if !defined( SCGI_LIBSCGI_HPP )
#define SCGI_LIBSCGI_HPP

#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>

namespace scgi {

std::tuple<std::size_t, std::size_t> check_netstring( std::byte const *, bool = true );
std::tuple<std::size_t, std::size_t> check_scgi( std::byte const *, bool = true );

std::unordered_map<std::string, std::string> parse_scgi_header( std::byte const *, std::size_t );

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
  void clear()
  {
    raw_header_size_ = 0;
    header_size = 0;
    start_size = 0;
    index.clear();
  }

  bool contains( key_type const &key ) const
  {
    // will this make it slow?
    if( index.empty() )
      return false;

    auto itr = std::lower_bound( index.begin(), index.end(), key,
        []( index_data_type const &a, key_type const &b ) -> bool { return a.first.compare( b ) < 0; } );

    if( ( itr != index.end() ) && ( key.compare( itr->first ) == 0 ) )
      return true;

    return false;
  }

  std::optional<value_type> const at( key_type const &key ) const
  {
    // will this make it slow?
    if( index.empty() )
      std::optional<value_type>{};

    auto itr = std::lower_bound( index.begin(), index.end(), key,
        []( index_data_type const &a, key_type const &b ) -> bool { return a.first.compare( b ) < 0; } );

    if( ( itr != index.end() ) && ( key.compare( itr->first ) == 0 ) )
      return itr->second;

    return std::optional<value_type>{};
  }

  void check_scgi() { std::tie( header_size, start_size ) = scgi::check_scgi( raw_header_ ); }
  std::size_t parse_scgi_header();

  std::byte *raw_header() { return raw_header_; }
  std::size_t raw_header_size() const { return raw_header_size_; }
  std::size_t raw_header_size( std::size_t s )
  {
    raw_header_size_ = s;
    return raw_header_size_;
  }
  std::size_t raw_header_max_size() const { return sizeof( raw_header_ ) / sizeof( std::byte ); }

private:
  std::byte raw_header_[1536];
  std::size_t raw_header_size_ = 0;

  std::size_t header_size = 0;
  std::size_t start_size = 0;

  index_type index;
};

} // namespace scgi

#endif // !defined( SCGI_LIBSCGI_HPP )
