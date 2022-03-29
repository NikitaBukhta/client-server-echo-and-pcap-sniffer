cmake_minimum_required(VERSION 3.0.0)

project(Client-Server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
# name of output file;
set(EXECUTABLE_APP_NAME "${PROJECT_NAME}-app")

add_executable(
    "${EXECUTABLE_APP_NAME}"
    main.cpp
)

message("Project: $CACHE{PROJECT_NAME}")
message("Current project version: $CACHE{CMAKE_PROJECT_VERSION}")