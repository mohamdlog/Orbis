cmake_minimum_required(VERSION 3.16)
project(orbis CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MAVSDK REQUIRED)

add_executable(orbis
    src/main.cpp
    src/monitor.cpp
)

target_include_directories(orbis PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(orbis PRIVATE
    MAVSDK::mavsdk
)