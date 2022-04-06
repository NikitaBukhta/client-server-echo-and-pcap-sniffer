#---------------------------------- SET STANDARDS ----------------------------------#
cmake_minimum_required(VERSION 3.0.0)

project(Client-Server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)

#------------------------------- SET SUBDIRECTORIES --------------------------------#

add_subdirectory(POSIX)

#-------------------------- SET EXECUTABLES AND LIBRARIES --------------------------#

# name of output file;
set(EXECUTABLE_APP_NAME $CACHE{CMAKE_PROJECT_NAME}-app)
set(
    LIBS
    posix_lib
)

add_executable(
    ${EXECUTABLE_APP_NAME}
    main.cpp
)

target_link_libraries(
    ${EXECUTABLE_APP_NAME}
    PUBLIC
    ${LIBS}
)