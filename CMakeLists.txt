cmake_minimum_required(VERSION 3.18)
project(InstallerDeepForge-Toolset)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)
include(FetchContent)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so .dylib)

# fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/DeepForge-Technology/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)
# jsoncpp
FetchContent_Declare(jsoncpp
GIT_REPOSITORY https://github.com/DeepForge-Technology/jsoncpp.git
GIT_TAG master
)
if(NOT jsoncpp_POPULATED)
  FetchContent_Populate(jsoncpp)
  add_subdirectory(${jsoncpp_SOURCE_DIR} ${jsoncpp_BINARY_DIR})
endif()
FetchContent_MakeAvailable(jsoncpp)
FetchContent_Declare(SQLite
  GIT_REPOSITORY https://github.com/DeepForge-Technology/SQLite.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SQLite)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG
      "/DWIN32 /D_WINDOWS /EHsc /WX /wd4355 /wd4251 /wd4250 /wd4996"
      CACHE STRING "Debug compiler flags" FORCE
    )
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")  
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -I /usr/local/include -I ../../include -DCURL_STATICLIB -Bstatic")
endif()

#target_include_directories(InstallerDeepForge-Toolset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)
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
    SQLite
    Threads::Threads
  )
elseif(APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++") 
  find_package(libzip REQUIRED)
  # find_package(CURL)
  # if(CURL_FOUND)
  #   include_directories(${CURL_INCLUDE_DIR})
  #   set(requiredlibs ${requiredlibs} ${CURL_LIBRARIES} )
  # else(CURL_FOUND)
  #   message(FATAL_ERROR "Could not find the CURL library and development files.")
  # endif(CURL_FOUND)
  FetchContent_Declare(libcurl
  GIT_REPOSITORY https://github.com/DeepForge-Technology/curl.git
  GIT_TAG master
  )
  FetchContent_MakeAvailable(libcurl)
  set(LIBRARIES
    fmt::fmt 
    jsoncpp_static
    libcurl_static
    libzip::zip
    SQLite
    Threads::Threads
  )
elseif(UNIX)
  FetchContent_Declare(libzip
    GIT_REPOSITORY https://github.com/DeepForge-Technology/libzip.git
    GIT_TAG master
  )
  FetchContent_MakeAvailable(libzip)
  FetchContent_Declare(libcurl
  GIT_REPOSITORY https://github.com/DeepForge-Technology/curl.git
  GIT_TAG master
  )
  FetchContent_MakeAvailable(libcurl)
  set(LIBRARIES
    fmt::fmt 
    jsoncpp
    libcurl_static
    SQLite
    zip
    Threads::Threads
  )
endif()

target_sources(InstallerDeepForge-Toolset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.rc)
target_link_libraries(InstallerDeepForge-Toolset 
  PRIVATE  
  ${LIBRARIES}
) 
# if(APPLE)
#   file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/InstallerDeepForge-Toolset.app/Contents)
#   file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/InstallerDeepForge-Toolset.app/Contents/MacOS)
#   file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/InstallerDeepForge-Toolset.app/Contents/Resources)
#   # Поиск всех файлов в исходной папке
#   file(GLOB ALL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src*")
#   # Копирование всех найденных файлов в целевую папку
#   file(COPY ${ALL_SOURCE_FILES} DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/InstallerDeepForge-Toolset.app/Contents/Resources")
#   file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/build/InstallerDeepForge-Toolset DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/InstallerDeepForge-Toolset.app/Contents/MacOS")
#   file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/InstallerDeepForge-Toolset.app/Contents")
# endif()