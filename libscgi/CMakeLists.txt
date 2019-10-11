project(libscgi_prj VERSION 0.0 LANGUAGES CXX)

add_library(libscgi libscgi.cpp)
target_include_directories(libscgi PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_executable(exp_reqmap exp_reqmap.cpp exp_libscgi.cpp)
target_link_libraries(exp_reqmap PRIVATE libscgi)

add_executable(tst_reqmap tst_reqmap.cpp)
target_link_libraries(tst_reqmap PRIVATE libscgi Catch2)

add_executable(mbm_reqmap mbm_reqmap.cpp exp_libscgi.cpp)
target_link_libraries(mbm_reqmap PRIVATE libscgi celero)
