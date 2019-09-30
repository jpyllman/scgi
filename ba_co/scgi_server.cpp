#include "scgi_server.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/fmt/bin_to_hex.h>

#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/detail/config.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include <regex>
#include <memory>

#if defined( SCGI_SERVER_TRACE_LOG )
#define SCGI_TRACE( a, b )                                                                                                         \
  {                                                                                                                                \
    a->trace( b );                                                                                                                 \
  }
#else
#define SCGI_TRACE( a, b )                                                                                                         \
  {}
#endif

namespace scgi {

static std::regex scgi_begin_re( R"((\d+):CONTENT_LENGTH)" );
constexpr static std::size_t small_buffer_size = 1536;

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

class session : public std::enable_shared_from_this<session>
{
public:
  session( boost::asio::ip::tcp::socket socket )
      : socket_( std::move( socket ) ), timer_( socket_.get_executor() ), small_buffer_( small_buffer_size )
  {
    log_ = spdlog::get( "ba_co" );
    if( log_ == nullptr )
      log_ = spdlog::null_logger_mt( "null" );

    SCGI_TRACE( log_, ">scgi::session::session()" );

    timer_.expires_at( std::chrono::steady_clock::time_point::max() );

    SCGI_TRACE( log_, "<scgi::session::session()" );
  }

  ~session()
  {
    SCGI_TRACE( log_, ">scgi::session::~session()" );
    SCGI_TRACE( log_, "<scgi::session::~session()" );
  }

  void start()
  {
    SCGI_TRACE( log_, ">scgi::session::start()" );

    co_spawn(
        socket_.get_executor(), [self = shared_from_this()] { return self->read_header(); }, boost::asio::detached );
    check_deadline( timer_ );

    SCGI_TRACE( log_, "<scgi::session::start() ->" );
  }

private:
  boost::asio::awaitable<void> read_header()
  {
    SCGI_TRACE( log_, ">scgi::session::read_header()" );

    try
    {
      timer_.expires_after( std::chrono::seconds( 10 ) );

      std::size_t bytes_read =
          co_await socket_.async_read_some( boost::asio::buffer( small_buffer_, 32 ), boost::asio::use_awaitable );
      log_->debug( "=scgi::session::read_header small_buffer_ {} {}", bytes_read,
          spdlog::to_hex( small_buffer_.begin(), small_buffer_.begin() + 32 ) );
      auto [bytes_in_head, size_of_start] = check_scgi( small_buffer_.data() );
      log_->debug( "=scgi::session::read_header scgi size {}, {}", bytes_in_head, size_of_start );

      if( size_of_start > 0 )
      {
        if( ( bytes_in_head + size_of_start + 1 ) <= small_buffer_size )
        {
          std::size_t n2 = co_await socket_.async_read_some(
              boost::asio::buffer( &small_buffer_[32], bytes_in_head - ( 32 - size_of_start ) + 1 ), boost::asio::use_awaitable );
          log_->debug( "=scgi::session::read_header small_buffer_ {} {}", n2,
              spdlog::to_hex(
                  small_buffer_.begin(), small_buffer_.begin() + ( ( bytes_in_head + size_of_start + 33 ) & 0xffffe0 ) ) );

          std::unordered_map<std::string, std::string> req =
              parse_scgi_header( ( small_buffer_.data() ) + size_of_start, bytes_in_head );
          log_->debug( "=scgi::session::read_header req {}", req.size() );
          for( auto [k, v] : req ) { log_->debug( "{} = {}", k, v ); }

          if( req.count( "SCGI" ) > 0 )
          {
            // let app code check header

            // change timeout before read a lot of data?
            // read rest of data if there is anything
            std::size_t content_length = 0;
            if( req.count( "CONTENT_LENGTH" ) > 0 )
              content_length = std::stoul( req.at( "CONTENT_LENGTH" ) );

            // let app execute request

            // ouput result
            std::string out{"Status: 200 OK\r\nContent-Type: text/plain;charset=utf-8\r\n\r\nI am here!"};
            co_await socket_.async_write_some( boost::asio::buffer( out ), boost::asio::use_awaitable );
            log_->debug( "=scgi::session::read_header out {}", spdlog::to_hex( out ) );
          }
          else
          {
            log_->error( "=scgi::session::read_header not a valid SCGI header" );
            stop();
          }
        }
        else
        {
          log_->error( "=scgi::session::read_header SCGI header larger then buffer" );
          stop();
        }
      }
      else
      {
        log_->error( "=scgi::session::read_header not a SCGI header" );
        stop();
      }
    }
    catch( std::exception &exc )
    {
      log_->debug( "=scgi::session::read_header exception [{}]", exc.what() );
      stop();
    }

    timer_.expires_at( std::chrono::steady_clock::time_point::max() );
    log_->debug( "=scgi::session::read_header end of session" );

    // session is over, do we really 'stop' the socket??
    stop();

    SCGI_TRACE( log_, "<scgi::session::read_header() ->" );
    co_return;
  }

  void stop()
  {
    SCGI_TRACE( log_, ">scgi::session::stop()" );

    socket_.close();
    timer_.cancel();

    SCGI_TRACE( log_, "<scgi::session::stop() ->" );
  }

  void check_deadline( boost::asio::steady_timer &deadline )
  {
    SCGI_TRACE( log_, ">scgi::session::check_deadline()" );

    deadline.async_wait( [self = shared_from_this(), &deadline]( boost::system::error_code const &ec ) {
      self->log_->debug( "=scgi::session::check_deadline lamda( {} )", ec.value() );

      if( !self->socket_.is_open() )
      {
        self->log_->debug( "=scgi::session::check_deadline lamda socket already closed" );
        return;
      }

      if( ( deadline.expiry() <= boost::asio::steady_timer::clock_type::now() ) )
      {
        self->log_->debug( "=scgi::session::check_deadline lamda expired so stop" );
        self->stop();
      }
      else
      {
        self->log_->debug( "=scgi::session::check_deadline lamda not expired so start over" );
        self->check_deadline( deadline );
      }
    } );

    SCGI_TRACE( log_, "<scgi::session::check_deadline() ->" );
  }

  boost::asio::ip::tcp::socket socket_;
  boost::asio::steady_timer timer_;

  std::vector<std::byte> small_buffer_;

  std::shared_ptr<spdlog::logger> log_;
};

boost::asio::awaitable<void> listener( boost::asio::ip::tcp::acceptor acceptor )
{
  auto log_ = spdlog::get( "ba_co" );
  if( log_ == nullptr )
    log_ = spdlog::null_logger_mt( "null" );
  SCGI_TRACE( log_, ">scgi::listener()" );

  for( ;; )
  {
    log_->debug( "=scgi::listener que new accept" );
    std::make_shared<session>( co_await acceptor.async_accept( boost::asio::use_awaitable ) )->start();
  }

  SCGI_TRACE( log_, "<scgi::listener() ->" );
  co_return;
}

int run_server( std::string const &addr, int port )
{
  auto log_ = spdlog::get( "ba_co" );
  if( log_ == nullptr )
    log_ = spdlog::null_logger_mt( "null" );
  log_->trace( ">scgi::run_server( {}, {} )", addr, port );

  boost::asio::io_context io_context( 1 );

  boost::asio::signal_set signals( io_context, SIGINT, SIGTERM );
  signals.async_wait( [&]( auto, auto ) { io_context.stop(); } );

  boost::asio::co_spawn(
      io_context,
      [&io_context, addr, port]() mutable {
        return listener( boost::asio::ip::tcp::acceptor(
            io_context, {boost::asio::ip::make_address_v4( addr ), static_cast<unsigned short>( port )} ) );
      },
      boost::asio::detached );

  io_context.run();

  log_->trace( "<scgi::run_server( {}, {} ) -> {}", addr, port, 0 );
  return 0;
}

} // namespace scgi
