#---------------------------------- SET STANDARDS ----------------------------------#

cmake_minimum_required(VERSION 3.0.0)

project(Client-Server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXE_LINKER_FLAGS -lpcap)

#------------------------------- SET SUBDIRECTORIES --------------------------------#

add_subdirectory(POSIX)
add_subdirectory(pcap)
add_subdirectory(sniffer)
add_subdirectory(server)
add_subdirectory(client)