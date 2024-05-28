cmake_minimum_required(VERSION 3.10)
project(NeuralinkCompression)

set(CMAKE_CXX_STANDARD 17)

# Указание путей к заголовочным файлам и библиотекам
set(FLAC_INCLUDE_DIR /usr/include)
set(FLAC_LIBRARY /usr/lib/x86_64-linux-gnu/libFLAC.so)
set(FLACPP_LIBRARY /usr/lib/x86_64-linux-gnu/libFLAC++.so)

include_directories(${FLAC_INCLUDE_DIR})

add_executable(encode encode.cpp)
target_link_libraries(encode ${FLAC_LIBRARY} ${FLACPP_LIBRARY})

add_executable(decode decode.cpp)
target_link_libraries(decode ${FLAC_LIBRARY} ${FLACPP_LIBRARY})

# Вывод найденных путей для диагностики
message(STATUS "FLAC_INCLUDE_DIR: ${FLAC_INCLUDE_DIR}")
message(STATUS "FLAC_LIBRARY: ${FLAC_LIBRARY}")
message(STATUS "FLACPP_LIBRARY: ${FLACPP_LIBRARY}")
