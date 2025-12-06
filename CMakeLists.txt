cmake_minimum_required(VERSION 4.0)
project(SSI)

set(CMAKE_CXX_STANDARD 20)

# Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/merco/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)

# Wyszukiwanaie Boost
find_package(Boost REQUIRED CONFIG COMPONENTS iostreams filesystem system)

# Tworzymy exe
add_executable(forestFire main.cpp gnuplot_iostream.h)

# Linkowanie bibliotek Boost
target_link_libraries(forestFire PRIVATE Boost::iostreams Boost::filesystem Boost::system)
