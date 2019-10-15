#include "libscgi.hpp"

#include <iostream>

int main()
{
  static constexpr const char raw_data[] = "24:CONTENT_LENGTH\0000\000SCGI\0001\000,";
  static constexpr const std::size_t raw_data_size = ( sizeof( raw_data ) / sizeof( char ) ) - 1;
  std::cout << "size of raw data = " << raw_data_size << "\n";

  scgi::request_header req;
  std::cout << "req.size() = " << req.size() << ", req.capacity() = " << req.capacity() << '\n';
  std::cout << "req.raw_header_size() = " << req.raw_header_size() << ", req.raw_header_max_size() = " << req.raw_header_max_size() << "\n";

  std::byte *in_data = req.raw_header();
  std::cout << "in_data = (ptr)" << in_data << ", req = (ptr)" << &req << "\n";
  for( int i = 0; i < raw_data_size; ++i ) in_data[i] = static_cast<std::byte>( raw_data[i] );
  req.raw_header_size( raw_data_size );
  std::cout << "req.raw_header_size() = " << req.raw_header_size() << ", req.raw_header_max_size() = " << req.raw_header_max_size() << "\n";

  auto [NO_bytes_in_header, NO_bytes_in_start] = scgi::check_scgi( in_data, true );
  std::cout << "in_data check -> NO_bytes_in_header = " << NO_bytes_in_header << ", NO_bytes_in_start = " << NO_bytes_in_start
            << '\n';

  auto NO_bytes_read = req.parse_scgi_header();
  std::cout << "parse header -> NO_bytes_read = " << NO_bytes_read << '\n';
  std::cout << "req.size() = " << req.size() << ", req.capacity() = " << req.capacity() << '\n';

  bool does_contain = req.contains( "SCGI" );
  std::cout << "req.contains( \"SCGI\" ) -> " << does_contain << '\n';
  if( does_contain )
    std::cout << "req.at( \"SCGI\" ) -> " << req.at( "SCGI" ).value_or( "" ) << "\n";
}
