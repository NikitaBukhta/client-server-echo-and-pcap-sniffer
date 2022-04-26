cmake_minimum_required(VERSION 3.0.0)

set(
    LIB_NAME
    "pcap_lib"
)

set(
    SRC
    src/pcap_wrapping.cpp
)

add_library(
    ${LIB_NAME}
    ${SRC}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC 
    include
)