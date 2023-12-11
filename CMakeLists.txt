cmake_minimum_required(VERSION 3.21)
project(InstallerDeepForge-Toolset)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD 20)
include (GNUInstallDirs)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so .dylib)
# set(ZIPPER_STATIC ON)
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

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
  SET(requiredlibs ${requiredlibs} ${CURL_LIBRARIES} )
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

# SQLite 3
FetchContent_Declare(SQLite
  GIT_REPOSITORY https://github.com/DeepForge-Technology/SQLite.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SQLite)
# libzip
FetchContent_Declare(libzip
  GIT_REPOSITORY https://github.com/DeepForge-Technology/libzip.git
  GIT_TAG master
)
FetchContent_MakeAvailable(libzip)

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
    urlmon
    jsoncpp_static
    ws2_32
    curl
    zip
    SQLite
  )
elseif(APPLE)
  set(LIBRARIES
    fmt::fmt 
    jsoncpp_static
    curl
    zip
    SQLite
  )
elseif(UNIX)
  set(LIBRARIES
    fmt::fmt 
    jsoncpp_static
    curl
    zip
    SQLite
  )
endif()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS}-DCURL_STATICLIB -static-libgcc -static-libstdc++ -Bstatic")
target_sources(InstallerDeepForge-Toolset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.rc)
target_link_libraries(InstallerDeepForge-Toolset 
  PRIVATE  
  ${LIBRARIES}
) 