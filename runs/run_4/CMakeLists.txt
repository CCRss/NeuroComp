cmake_minimum_required(VERSION 3.10)
project(NeuralinkCompression)

set(CMAKE_CXX_STANDARD 17)

find_library(BROTLI_ENC brotlienc)
find_library(BROTLI_DEC brotlidec)
find_library(BROTLI_COMMON brotlicommon)

include_directories(/usr/include)

add_executable(encode encode.cpp)
target_link_libraries(encode ${BROTLI_ENC} ${BROTLI_COMMON})

add_executable(decode decode.cpp)
target_link_libraries(decode ${BROTLI_DEC} ${BROTLI_COMMON})
