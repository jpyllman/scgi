add_subdirectory(Clara)

option(CATCH_BUILD_EXAMPLES "Build documentation examples" OFF)
option(CATCH_INSTALL_DOCS "Install documentation alongside library" OFF)
option(CATCH_INSTALL_HELPERS "Install contrib alongside library" OFF)
add_subdirectory(Catch2)

add_subdirectory(Celero)

add_subdirectory(fmt)

add_subdirectory(spdlog)

