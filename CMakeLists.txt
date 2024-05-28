cmake_minimum_required(VERSION 3.10)
project(NeuralinkCompression)

set(CMAKE_CXX_STANDARD 17)

add_executable(encode encode.cpp)
add_executable(decode decode.cpp)
