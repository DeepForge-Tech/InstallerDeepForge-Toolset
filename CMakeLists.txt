cmake_minimum_required(VERSION 3.21)
project(InstallerDeepForge-Toolset)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD 20)
include (GNUInstallDirs)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so .dylib)
set(ZIPPER_STATIC ON)
# find_package(ZLIB)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
# fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

# jsoncpp
FetchContent_Declare(jsoncpp
  GIT_REPOSITORY https://github.com/DeepForge-Technology/jsoncpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(jsoncpp)
# find_package(jsoncpp REQUIRED)
# Zlib
FetchContent_Declare(zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG master
)
FetchContent_MakeAvailable(zlib)
set(ZLIB_INCLUDE_DIRS ${zlib_SOURCE_DIR})
set(ZLIB_LIBRARY zlibstatic CACHE STRING "")
set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
set(ZLIB_FOUND 1)
add_library(ZLIB::ZLIB ALIAS zlibstatic)
# find_package(ZLIB)
# CURL
FetchContent_Declare(curl
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG master
)
FetchContent_MakeAvailable(curl)

# SQLite 3
FetchContent_Declare(SQLite
  GIT_REPOSITORY https://github.com/DeepForge-Technology/SQLite.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SQLite)
# Zipper
FetchContent_Declare(Zipper
  GIT_REPOSITORY https://github.com/sebastiandev/zipper.git
  GIT_TAG master
  GIT_SUBMODULES_RECURSE TRUE
)
FetchContent_MakeAvailable(Zipper)


include_directories("./include")
set(SOURCE_FILES
    ./src/InstallerDeepForge_Toolset.cpp
    ./src/DatabaseConnect.cpp
    ./src/DatabaseConnect.hpp
)
add_executable(InstallerDeepForge-Toolset ${SOURCE_FILES})

if (WIN32)
  set(LIBRARIES
    fmt::fmt 
    staticZipper
    urlmon
    jsoncpp_static
    ws2_32
    libcurl_static
    zlibstatic
    SQLite
  )
elseif(APPLE)
  set(LIBRARIES
    fmt::fmt 
    staticZipper
    jsoncpp_static
    libcurl_static
    zlibstatic
    SQLite
  )
elseif(UNIX)
  set(LIBRARIES
    fmt::fmt 
    staticZipper
    jsoncpp_static
    libcurl_static
    zlibstatic
    SQLite
  )
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS}-DCURL_STATICLIB -static-libgcc -static-libstdc++ -Bstatic")
target_sources(InstallerDeepForge-Toolset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.rc)
target_link_libraries(InstallerDeepForge-Toolset 
  PRIVATE  
  ${LIBRARIES}
) 