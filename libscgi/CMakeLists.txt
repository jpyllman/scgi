project(libscgi_prj VERSION 0.0 LANGUAGES CXX)

add_library(libscgi libscgi.cpp)

add_executable(tst_reqmap tst_reqmap.cpp)
target_link_libraries(tst_reqmap PRIVATE libscgi)
