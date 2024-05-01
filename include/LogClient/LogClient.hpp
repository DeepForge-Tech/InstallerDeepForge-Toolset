#ifndef LOG_CLIENT
#define LOG_CLIENT
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <map>
#include <exception>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <stdio.h>
#include <chrono>
#include <ctime>

#define LOGSERVER_URL "https://enthusiastic-crow-kilt.cyclic.app/"

namespace LogClient
{
    size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    class Client {
    public:
        CURL *curl;
        CURLcode res;
        // string type
        std::string readBuffer;
        std::string GET_URL = "https://enthusiastic-crow-kilt.cyclic.app/";
//        Value data;
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string logsDir = currentDir.generic_string() + "/logs";
        std::string logPath = logsDir + "/log.txt";
        std::map<std::string, std::string> logInformation = {
                {"Debug", "[DEBUG]"},
                {"Info", "[INFO]"},
                {"Error", "[ERROR]"},
                {"Warning", "[WARNING]"},
                {"Success", "[SUCCESS]"}
        };

        int POST(const char *url,const Json::Value& data);

        int GET(const char *url);

        int writeLog(std::basic_string<char, std::char_traits<char>, std::allocator<char>> log_text, const char *type);

        std::string getTime();

        Client() {
            curl = curl_easy_init();
        }

    private:
    };
}
#endif