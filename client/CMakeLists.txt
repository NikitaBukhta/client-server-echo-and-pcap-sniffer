#---------------------------------- SET STANDARDS ----------------------------------#

cmake_minimum_required(VERSION 3.0.0)

project(server)

set(CXX_STANDARD_REQUIRED 14)

set(EXECUTABLE_NAME "client-app")

add_executable(
    ${EXECUTABLE_NAME}
    client-app.cpp    
)

#----------------------------------- ADD LIBRARY -----------------------------------#

# name of lib we create;
set(
    LIB_NAME
    "client_class"
)

set(
    SRC
    src/client.cpp
)

# libs we need for our lib;
set(
    INCLUDED_LIBS
    posix_lib
)

add_library(
    ${LIB_NAME}
    ${SRC}
)

# include directories with header files;
target_include_directories(
    ${LIB_NAME}
    PUBLIC 
    include
)

# link libraries we need for project
target_link_libraries(
    ${LIB_NAME} 
    PUBLIC 
    ${INCLUDED_LIBS}
)

#----------------------------------- INCLUDE LIBRAREIS -----------------------------------#

# there contain all libs that needed for executable file
set(
    ALL_LIBS
    ${LIB_NAME}
    ${INCLUDED_LIBS}
)

# all this libs;
target_link_libraries(
    ${EXECUTABLE_NAME}
    PUBLIC
    ${ALL_LIBS}
)