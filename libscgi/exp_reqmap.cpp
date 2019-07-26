#include "libscgi.hpp"

#include <iostream>

int main()
{
  scgi::request_map req;
  std::cout << req.size() << ", " << req.capacity() << '\n';

  std::vector<std::byte> in_data{28};
  static const char *x = "24:CONTENT_LENGTH\0000\000SCGI\0001\000,";
  for( int i = 0; i < 28; ++i ) in_data[i] = static_cast<std::byte>( x[i] );

  auto [u, v] = scgi::check_scgi( in_data.data() );
  std::cout << "in_data check -> " << u << ", " << v << '\n';

  auto z = req.parse_scgi_header( in_data.data() + v, u );
  std::cout << "in_data NO bytes read -> " << z << '\n';

  bool y = req.contains( "SCGI" );
  std::cout << "req.contains() -> " << y << '\n';
  if( y )
    std::cout << "req.at() -> " << req.at( "SCGI" ) << "\n";
}
