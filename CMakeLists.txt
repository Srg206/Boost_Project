cmake_minimum_required(VERSION 3.20)
project(ClientApp)

set(Boost_INCLUDE_DIR C:/boost_1_84_0)
set(Boost_LIBRARY_DIR C:/boost_1_84_0/stage/lib)
##find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Boost COMPONENTS REQUIRED)


include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})




add_subdirectory(Client)
add_subdirectory(Server)

