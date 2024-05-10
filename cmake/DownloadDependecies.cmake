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

FetchContent_MakeAvailable(jsoncpp)
FetchContent_Declare(SQLite
  GIT_REPOSITORY https://github.com/DeepForge-Technology/SQLite.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SQLite)
FetchContent_Declare(Logger
  GIT_REPOSITORY https://github.com/DeepForge-Tech/Logger.git
  GIT_TAG master
)
FetchContent_MakeAvailable(Logger)

FetchContent_Declare(libcurl
  GIT_REPOSITORY https://github.com/DeepForge-Technology/curl.git
  GIT_TAG master
)
FetchContent_MakeAvailable(libcurl)


FetchContent_Declare(miniz
  GIT_REPOSITORY https://github.com/DeepForge-Tech/miniz.git
  GIT_TAG master
)
FetchContent_MakeAvailable(miniz)

FetchContent_Declare(Progressbar
  GIT_REPOSITORY https://github.com/DeepForge-Tech/Progressbar.git
  GIT_TAG master
)
FetchContent_MakeAvailable(Progressbar)

FetchContent_Declare(DatabaseAPI
  GIT_REPOSITORY https://github.com/DeepForge-Tech/DatabaseAPI.git
  GIT_TAG master
)
FetchContent_MakeAvailable(DatabaseAPI)