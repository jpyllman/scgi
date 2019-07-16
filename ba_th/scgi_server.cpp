#include "scgi_server.hpp"

namespace scgi {

template <typename SessionHandler>
class server
{
public:
  server( std::string const &addr, int port ) {}

protected:
};

class session : public std::enable_shared_from_this<session>
{
public:
private:
};

int run_server( std::string const &addr, int port )
{
  server<session> s( addr, port );

  return 0;
}

} // namespace scgi
