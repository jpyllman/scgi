project(ba_co_prj VERSION 0.0 LANGUAGES CXX)

find_package(Boost 1.70.0 REQUIRED COMPONENTS system thread coroutine)

add_executable(ba_co main.cpp scgi_server.cpp)
target_compile_features(ba_co PUBLIC cxx_std_20)
target_link_libraries(ba_co PRIVATE project_warnings clara fmt spdlog Boost::system Boost::thread Boost::coroutine)
