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

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64") # for desktop
    set(architecture "x64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
    set(architecture "x64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64") # for tx2
    set(architecture "arm64")
endif()
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/${architecture}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Download dependecies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DownloadDependecies.cmake)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG
      "/DWIN32 /D_WINDOWS /EHsc /WX /wd4355 /wd4251 /wd4250 /wd4996  /MT"
      CACHE STRING "Debug compiler flags" FORCE
    )
    set(CMAKE_CXX_FLAGS_RELEASE
      "/DWIN32 /D_WINDOWS /EHsc  /MT"
      CACHE STRING "Release compiler flags" FORCE
    )
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")  
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -I /usr/local/include -I ../../include -DCURL_STATICLIB -Bstatic")
endif()

#target_include_directories(InstallerDeepForge-Toolset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCE_FILES
  src/InstallerDeepForge-Toolset.cpp
  include/InstallerDeepForge-Toolset/InstallerDeepForge-Toolset.hpp
)
add_executable(InstallerDeepForge-Toolset ${SOURCE_FILES})
if (WIN32)
  set(USE_WINDOWS_SSPI ON)
  set(USE_SCHANNEL ON)
  set(LIBRARIES
    fmt::fmt 
    urlmon
    jsoncpp_static
    miniz
    ws2_32
    SQLite
    Logger
    Progressbar
    DatabaseAPI
    Threads::Threads
  )
elseif(APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++") 
  set(LIBRARIES
    fmt::fmt 
    jsoncpp_static
    libcurl_static
    miniz
    SQLite
    Logger
    Progressbar
    DatabaseAPI
    Threads::Threads
  )
elseif(UNIX)
  set(LIBRARIES
    fmt::fmt 
    jsoncpp_static
    libcurl_static
    SQLite
    miniz
    Logger
    Progressbar
    DatabaseAPI
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
