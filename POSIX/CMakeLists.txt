cmake_minimum_required(VERSION 3.0.0)

set(
    LIB_NAME
    "posix_lib"
)

set(
    SRC
    src/posix_wrapping.cpp
    src/posixError.cpp
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