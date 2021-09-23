cmake_minimum_required(VERSION 3.18.1)

project("hwcpipe_exporter")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(HWCPipe)

add_library(hwcpipe_exporter SHARED main.cpp)

find_library(log-lib log)

target_link_libraries(
        hwcpipe_exporter

        ${log-lib}
        hwcpipe)