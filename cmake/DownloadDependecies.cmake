# fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/DeepForge-Technology/fmt.git
  GIT_TAG master
)
# if(NOT fmt_POPULATED)
#   FetchContent_Populate(fmt)

#   add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(fmt)

# jsoncpp
FetchContent_Declare(jsoncpp
GIT_REPOSITORY https://github.com/DeepForge-Technology/jsoncpp.git
GIT_TAG master
)
# if(NOT jsoncpp_POPULATED)
#   FetchContent_Populate(jsoncpp)

#   add_subdirectory(${jsoncpp_SOURCE_DIR} ${jsoncpp_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(jsoncpp)

# SQLite
FetchContent_Declare(SQLite
  GIT_REPOSITORY https://github.com/DeepForge-Technology/SQLite.git
  GIT_TAG master
)
# if(NOT SQLite_POPULATED)
#   FetchContent_Populate(SQLite)

#   add_subdirectory(${SQLite_SOURCE_DIR} ${SQLite_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(SQLite)

# Logger
FetchContent_Declare(Logger
  GIT_REPOSITORY https://github.com/DeepForge-Tech/Logger.git
  GIT_TAG master
)
# if(NOT Logger_POPULATED)
#   FetchContent_Populate(Logger)

#   add_subdirectory(${Logger_SOURCE_DIR} ${Logger_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(Logger)

# libcurl
FetchContent_Declare(libcurl
  GIT_REPOSITORY https://github.com/DeepForge-Technology/curl.git
  GIT_TAG master
)
# if(NOT libcurl_POPULATED)
#   FetchContent_Populate(libcurl)

#   add_subdirectory(${libcurl_SOURCE_DIR} ${libcurl_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(libcurl)

# miniz
FetchContent_Declare(miniz
  GIT_REPOSITORY https://github.com/DeepForge-Tech/miniz.git
  GIT_TAG master
)
# if(NOT miniz_POPULATED)
#   FetchContent_Populate(miniz)

#   add_subdirectory(${miniz_SOURCE_DIR} ${miniz_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(miniz)

# Progressbar
FetchContent_Declare(Progressbar
  GIT_REPOSITORY https://github.com/DeepForge-Tech/Progressbar.git
  GIT_TAG master
)
# if(NOT Progressbar_POPULATED)
#   FetchContent_Populate(Progressbar)

#   add_subdirectory(${Progressbar_SOURCE_DIR} ${Progressbar_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(Progressbar)

# DatabaseAPI
FetchContent_Declare(DatabaseAPI
  GIT_REPOSITORY https://github.com/DeepForge-Tech/DatabaseAPI.git
  GIT_TAG master
)
# if(NOT DatabaseAPI_POPULATED)
#   FetchContent_Populate(DatabaseAPI)

#   add_subdirectory(${DatabaseAPI_SOURCE_DIR} ${DatabaseAPI_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
FetchContent_MakeAvailable(DatabaseAPI)