cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ADCP LANGUAGES C CXX)

# Set this to enable CPP_CHECK
set(CMAKE_CXX_CPPCHECK "cppcheck")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (GNUInstallDirs)
#this sets up some default directory paths

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

#define sources
include(src/CMakeLists.txt)

