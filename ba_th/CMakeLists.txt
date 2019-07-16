project(ba_th_prj VERSION 0.0 LANGUAGES CXX)

find_package(Boost 1.70.0 REQUIRED COMPONENTS system thread)

add_executable(ba_th main.cpp scgi_server.cpp)
target_compile_features(ba_th PUBLIC cxx_std_14)
target_link_libraries(ba_th PRIVATE project_warnings clara spdlog Boost::system Boost::thread)
