#include "scgi_server.hpp"

#include <spdlog/sinks/rotating_file_sink.h>

#include <clara/clara.hpp>

#include <iostream>

struct cli_data
{
  bool show_help = false;
  bool show_version = false;

  std::string addr = "127.0.0.1";
  int port = 12345;
};

bool parse_CLI( int NOArgs, char *Args[], cli_data &data )
{

  auto cli = clara::Opt( data.addr, "addr" )["--addr"]( "ipv4 address to listen to, default is 127.0.0.1" ).optional() |
             clara::Opt( data.port, "port" )["--port"]( "ipv4 port to listen to, default is 12345" ).optional() |
             clara::Help( data.show_help ) | clara::Opt( data.show_version )["--version"]( "show version of ba_th" ).optional();

  auto cli_result = cli.parse( clara::Args( NOArgs, Args ) );

  if( !cli_result )
  {
    fmt::print( "ba_th cli error: {}\n", cli_result.errorMessage() );
    return false;
  }

  if( data.show_help )
  {
    // std::ostringstream oss;
    // oss << cli;
    // fmt::print( oss.str() );q
    std::cout << cli;

    return false;
  }

  if( data.show_version )
  {
    fmt::print( "ba_th v{}\n", "0.0" );
    return false;
  }

  return true;
}

int main( int NOArgs, char *Args[], char *[] )
{
  cli_data config;
  if( parse_CLI( NOArgs, Args, config ) )
  {
    auto log_ = spdlog::rotating_logger_mt( "ba_th", "ba_th.log", 5 * 1024 * 1024, 3 );
    log_->set_pattern( "[%Y-%m-%d %H:%M:%S.%e][%n][%L][%t] %v" );
    log_->set_level( spdlog::level::trace );
    log_->info( "ba_th started" );

    scgi::run_server( config.addr, config.port );

    log_->info( "ba_th stopped" );
  }
}
