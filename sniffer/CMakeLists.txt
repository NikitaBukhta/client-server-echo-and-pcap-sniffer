cmake_minimum_required(VERSION 3.0.0)

set(
    LIB_NAME
    "sniffer_lib"
)

set(
    SRC
    src/sniffer.cpp
)

set(
    INCLUDED_LIBS
    posix_lib
    pcap_lib
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

target_link_libraries(
    ${LIB_NAME}
    PUBLIC
    ${INCLUDED_LIBS}
)