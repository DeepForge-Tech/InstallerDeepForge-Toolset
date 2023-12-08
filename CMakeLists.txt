cmake_minimum_required(VERSION 3.21)
project(InstallerDeepForge-Toolset)
set(CMAKE_CXX_STANDARD 20)
include (GNUInstallDirs)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so .dylib)
set(ZIPPER_STATIC ON)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package (SQLite3)
find_package(ZLIB)
if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
endif (SQLITE3_FOUND)

find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
FetchContent_Declare(Zipper
  GIT_REPOSITORY https://github.com/sebastiandev/zipper.git
  GIT_TAG master
  GIT_SUBMODULES_RECURSE TRUE
)
FetchContent_MakeAvailable(Zipper)

set(Zipper_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/Zipper)
# find_package(Zipper REQUIRED)

include_directories("./include")
set(SOURCE_FILES
    ./src/InstallerDeepForge_Toolset.cpp
    ./src/DatabaseConnect.cpp
    ./src/DatabaseConnect.hpp
)
add_executable(InstallerDeepForge-Toolset ${SOURCE_FILES})

if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/Windows) 
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${JSONCPP_INCLUDE_DIR}\"")           
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/MacOS)
elseif(UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/Linux)
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB -static-libgcc -static-libstdc++")
target_sources(InstallerDeepForge-Toolset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.rc)
target_link_libraries(InstallerDeepForge-Toolset 
    PRIVATE  
    fmt::fmt 
    sqlite3
    staticZipper
    urlmon
    jsoncpp
    ws2_32
    curl
)