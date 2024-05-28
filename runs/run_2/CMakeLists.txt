cmake_minimum_required(VERSION 3.10)
project(NeuralinkCompression)

set(CMAKE_CXX_STANDARD 17)

find_package(ZLIB REQUIRED)

add_executable(encode encode.cpp)
target_link_libraries(encode ZLIB::ZLIB)

add_executable(decode decode.cpp)
target_link_libraries(decode ZLIB::ZLIB)
