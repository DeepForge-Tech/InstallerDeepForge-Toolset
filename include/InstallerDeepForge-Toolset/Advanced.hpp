/*  The MIT License (MIT)
    ============================================================================

    ██████╗ ███████╗███████╗██████╗ ███████╗ ██████╗ ██████╗  ██████╗ ███████╗
    ██╔══██╗██╔════╝██╔════╝██╔══██╗██╔════╝██╔═══██╗██╔══██╗██╔════╝ ██╔════╝
    ██║  ██║█████╗  █████╗  ██████╔╝█████╗  ██║   ██║██████╔╝██║  ███╗█████╗
    ██║  ██║██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██║   ██║██╔══██╗██║   ██║██╔══╝
    ██████╔╝███████╗███████╗██║     ██║     ╚██████╔╝██║  ██║╚██████╔╝███████╗
    ╚═════╝ ╚══════╝╚══════╝╚═╝     ╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝

    ████████╗ ██████╗  ██████╗ ██╗     ███████╗███████╗████████╗
    ╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝██╔════╝╚══██╔══╝
       ██║   ██║   ██║██║   ██║██║     ███████╗█████╗     ██║
       ██║   ██║   ██║██║   ██║██║     ╚════██║██╔══╝     ██║
       ██║   ╚██████╔╝╚██████╔╝███████╗███████║███████╗   ██║
       ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚══════╝   ╚═╝

    ============================================================================
    Copyright (c) 2024 DeepForge Technology
    ============================================================================
    Organization: DeepForge Technology
    ============================================================================
    Author: Blackflame576
    ============================================================================
    Created: 4 Juny 2023
    ============================================================================
*/
// Importing Libraries
#ifndef ADVANCED_HPP_
#define ADVANCED_HPP_

#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <Progressbar.hpp>
#include <cctype>
#include <fstream>
#include <functional>
#include <json/json.h>
#include <time.h>
#include <cstdint>
#include <chrono>
#include <vector>
#include <Database/DatabaseConnect.hpp>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <Logger/Logger.hpp>
#include <future>
#include <miniz/miniz.h>
// Checking the name of the operating system and importing the necessary libraries for this system
#if defined(__linux__)
#include <curl/curl.h>
#include <unistd.h>
#include <regex>

#elif __APPLE__
#include <unistd.h>
#include <curl/curl.h>

#elif _WIN32
#include <conio.h>
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shlwapi.lib")
#include "shlobj.h"
#endif

#define NameProgram "InstallerDeepForge-Toolset"
#define __version__ "0.1"
#define __channel__ "stable\\latest"
#define APPINSTALLER_DB_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/AppInstaller.db"
#define DB_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db"
#define Locale_RU_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/locale_ru.json"
#define Locale_EN_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/locale_en.json"

#if defined(__linux__)
#define URL_DESKTOP_SYMLINK "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/DeepForgeToolset.desktop"
#define NameVersionTable "DeepForgeToolset_Linux"
#define UpdateManagerTable "UpdateManager_Linux"
#define PATHMAN_AMD64_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-linux-amd64"
#define PATHMAN_ARM64_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-linux-armv8"
#define SERVICE_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/DeepForge-UpdateManager.service"
#define SHELL_SCRIPT_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/InstallLibraries_Linux.sh"
#define OS_NAME "Linux"

#elif __APPLE__

#define OS_NAME "macOS"

#elif _WIN32

#define OS_NAME "Windows"
#define UpdateManagerTable "UpdateManager_Windows"
#define NameVersionTable "DeepForgeToolset_Windows"
#define PathmanURL_AMD64 "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-windows-amd64.exe"
#define PathmanURL_ARM64 "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-windows-amd64.exe"

#endif

// Variables
// int type
int Percentage;
int TempPercentage = 0;
int result;
int output_func;
int defaultChannel = 1;
// enum classes
enum class LanguageChoices
{
    RUSSIAN,
    ENGLISH
};
// bool type
bool Updating = true;
bool withProgress = true;
// double type
double LastSize;
double LastTotalSize;
double DownloadSpeed;
// map type
std::map<int, std::string> EnumerateChannels;
std::map<std::string, std::string> Channels;

/* The `replaceAll` function is a utility function that replaces all occurrences of a substring `from` with another substring `to` in a given string `str`. */
std::string replaceAll(std::string str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
// string type
std::string AllChannels[2] = {"stable", "beta"};
const std::string TrueVarious[3] = {"yes", "y", "1"};
std::string ProjectDir = std::filesystem::current_path().generic_string();
std::string haveString = "";
std::string new_sentence;
std::string LangReadySet;
std::string InstallDelimiter = "========================================================";
std::string Language;
std::string SelectPackages;
std::string Answer;
std::string MODE = "DEV";
// Boolean type
bool Install;

#if defined(__APPLE__)
std::string Architecture;
#if defined(_M_AMD64)
Architecture = "amd64";
#elif __arm__ || __aarch64__ || _M_ARM64
Architecture = "arm64";
#endif
std::string OrganizationFolder;
std::string DesktopPath;
std::string ApplicationDir;
std::string TempFolder;
std::string LocaleDir;
std::string UpdateManagerFolder;
std::string DatabasePath;
std::string LogPath;
const std::string NewUpdateManagerFolder = OrganizationFolder + "/UpdateManager";
#elif __linux__
std::string Architecture;
#if defined(_M_AMD64)
Architecture = "amd64";
#elif __arm__ || __aarch64__ || _M_ARM64
Architecture = "arm64";
#endif
std::string NameDistribution;
std::string PackageManager;
char *UserFolder = getenv("USER");
const std::string DesktopPath = std::string(UserFolder) + "/Desktop";
const std::string OrganizationFolder = "/usr/bin/DeepForge";
const std::string ApplicationDir = OrganizationFolder + "/DeepForge-Toolset";
const std::string UpdateManagerFolder = OrganizationFolder + "/UpdateManager";
std::string TempFolder = ApplicationDir + "/Temp";
const std::string LocaleDir = ApplicationDir + "/locale";
std::string DatabasePath = TempFolder + "/Versions.db";
const std::string NewUpdateManagerFolder = OrganizationFolder + "/UpdateManager";
#elif _WIN32
#if defined(_M_AMD64)
std::string Architecture = "amd64";
#elif __arm__ || __aarch64__ || _M_ARM64
std::string Architecture = "arm64";
#endif
char *UserFolder = getenv("USERPROFILE");
const std::string OrganizationFolder = "C:\\ProgramData\\DeepForge";
const std::string DesktopPath = std::string(UserFolder) + "\\Desktop";
std::string ApplicationDir = "C:\\ProgramData\\DeepForge\\DeepForge-Toolset";
std::string TempFolder = ApplicationDir + "\\Temp";
std::string DatabasePath = TempFolder + "\\Versions.db";
std::string LocaleDir = ApplicationDir + "\\locale";
const std::string NewUpdateManagerFolder = OrganizationFolder + "\\UpdateManager";
#endif
// init classes
Logger::Logging logger;
Bar::ProgressBar_v1 progressbar;
Json::Value translate;
DB::Database database;

void WriteInformation(std::string version)
{
    try
    {
        std::string NameTable = "DeepForgeToolset_" + std::string(OS_NAME);
        std::map<std::string, std::string> ApplicationColumns = {
            {"Name", "TEXT"},
            {"Version", "TEXT"},
            {"NameTable", "TEXT"}};
        std::map<std::string, std::string> ApplicationFields = {
            {"Name", "DeepForge-Toolset"},
            {"Version", version},
            {"NameTable", NameTable}};
        std::string pathFile = NewUpdateManagerFolder + "\\AppInformation.db";
        DB::Database AppInformationDB;
        /* The bellow code is checking if a file exists at the specified path. If the file does not exist, it creates a new file and writes an empty string to it. Then, it opens a database connection using the file as the database path. It checks if a table named "Applications" exists in the database. If the table does not exist, it creates the table with the specified columns. Finally, it inserts values into the "Applications" table. */
        if (std::filesystem::exists(pathFile) == false)
        {
            std::ofstream file(pathFile);
            file << "";
            file.close();
        }
        AppInformationDB.open(&pathFile);
        /* The bellow code is creating a table called "Applications" in the AppInformationDB database using the CreateTable method. It then inserts values into the "Applications" table using the InsertValuesToTable method. The boolean variable "exists" is used to store the result of the CreateTable method, indicating whether the table creation was successful or not. The integer variable "result" is used to store the number of rows affected by the InsertValuesToTable method. */
        bool existsTable = AppInformationDB.CreateTable("Applications", ApplicationColumns);
        int existsValue = AppInformationDB.ExistNameAppInTable("Applications", "DeepForge-Toolset");
        /* The code is checking if a table called "Applications" exists in the database. If the table does not exist (existsTable == -1), it inserts values into the table using the AppInformationDB.InsertValuesToTable() method. If the table does exist, it removes an application called "DeepForge-Toolset" from the table using the AppInformationDB.RemoveApplicationFromTable() method, and then inserts values into the table using the AppInformationDB.InsertValuesToTable() method. */
        if (existsTable == false)
        {
            result = AppInformationDB.InsertValuesToTable("Applications", ApplicationFields);
        }
        else
        {
            AppInformationDB.RemoveApplicationFromTable("Applications", "DeepForge-Toolset");
            result = AppInformationDB.InsertValuesToTable("Applications", ApplicationFields);
        }
    }
    catch (std::exception &error)
    {
        // Error output
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "WriteInformation()", error.what());
        std::cerr << error.what() << std::endl;
    }
}

void UploadInformation()
{
    // std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    Channels = database.GetAllVersionsFromDB(NameVersionTable, "Channel", Architecture);
    int size = (sizeof(AllChannels) / sizeof(AllChannels[0]));
    int n = 1;
    /* The code is iterating over the `AllChannels` array and checking if each channel exists in
    the `Channels` map. If a channel exists, it prints the channel number and name, and inserts
    the channel into the `EnumerateChannels` map. If the channel is "stable\latest", it sets the
    `defaultChannel` variable to the current channel number. */
    for (int i = 0; i < size; i++)
    {

        if (Channels.find(AllChannels[i]) != Channels.end())
        {
            EnumerateChannels.insert(std::pair<int, std::string>(n, AllChannels[i]));
            if (AllChannels[i] == "stable")
            {
                defaultChannel = n;
            }
            n++;
        }
    }
}

#if defined(__APPLE__) || defined(__linux__)
/**
 * The function `CallbackProgress` is a callback function used to track the progress of a download
 * and update a progress bar accordingly.
 *
 * @param ptr The `ptr` parameter is a pointer to user-defined data that can be passed to the
 * callback function. It allows you to pass additional information or context to the callback
 * function if needed.
 * @param TotalToDownload The total size of the file to be downloaded in bytes.
 * @param NowDownloaded The amount of data that has been downloaded so far.
 * @param TotalToUpload The total size of the data to be uploaded in bytes.
 * @param NowUploaded The parameter "NowUploaded" represents the amount of data that has been
 * uploaded so far. It is a double data type.
 *
 * @return an integer value of 0.
 */
int CallbackProgress(void *ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    if (TotalToDownload <= 0.0)
    {
        return 0;
    }
    // if (withProgress == true)
    // {
    // double DownloadSpeed;
    Percentage = static_cast<float>(NowDownloaded) / static_cast<float>(TotalToDownload) * 100;
    /* The bellow code is checking if the `TempPercentage` is not equal to `Percentage` and is less
    than or equal to 100. If this condition is true, it retrieves the download speed using
    `curl_easy_getinfo` and updates a progress bar using the `progressbar.Update` function. It
    also updates some variables (`LastDownloadSpeed`, `LastSize`, `LastTotalSize`, and
    `TempPercentage`) with the current values. */
    if (TempPercentage != Percentage && TempPercentage <= 100)
    {
        progressbar.Update(NowDownloaded, TotalToDownload);
        LastSize = NowDownloaded;
        LastTotalSize = TotalToDownload;
        TempPercentage = Percentage;
    }
    // }
    return 0;
}
/**
 * The function "WriteData" writes data from a pointer to a file stream.
 *
 * @param ptr ptr is a pointer to the data that needs to be written to the file. It points to the
 * starting address of the data.
 * @param size The size parameter specifies the size of each element to be written.
 * @param nmemb The parameter "nmemb" stands for "number of members". It represents the number of
 * elements, each with a size of "size", that you want to write to the file.
 * @param stream The stream parameter is a pointer to a FILE object, which represents the file
 * stream that the data will be written to.
 *
 * @return the number of elements successfully written to the file stream.
 */
size_t WriteData(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t WriteProcess = fwrite(ptr, size, nmemb, stream);
    return WriteProcess;
}
#endif

// Function to create a string with two application names
std::string NewString(std::string sentence)
{
    std::string new_sentence = "";
    // If the string is empty, then the first application name is added.
    if (haveString == "")
    {
        haveString = sentence;
        return new_sentence;
    }
    else
    {
        // Formatting string with two columns
        /* If the string already contains the name of the application,
        then the second name of the application is added and the formatted string is returned */
        new_sentence = fmt::format("{:<40} {:<15}\n", haveString, sentence);
        haveString = "";
        return new_sentence;
    }
}
// Method of make string to lower
std::string to_lower(std::string sentence)
{
    std::string new_sentence = "";
    for (int i = 0; i < sentence.length(); i++)
    {
        char ch = sentence[i];
        // cout << ch << endl;
        ch = tolower(ch);
        new_sentence += ch;
    }
    return new_sentence;
}
// Function for check of answer
bool CheckAnswer(std::string answer)
{
    bool status;
    // string Answer = to_lower(answer);

    std::string Answer = answer;
    for (int i = 0; i < TrueVarious->size(); i++)
    {
        if (Answer == TrueVarious[i] || Answer.empty() || Answer == "\n" || Answer == "да" || Answer == "ДА" || Answer == "Да")
        {
            status = true;
            break;
        }
    }
    return status;
}

#endif