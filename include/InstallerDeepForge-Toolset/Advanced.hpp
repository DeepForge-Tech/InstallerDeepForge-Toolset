/*  GNU GENERAL PUBLIC LICENSE
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
// #include <map>
#include <unordered_map>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <Progressbar/Progressbar.hpp>
#include <cctype>
#include <fstream>
#include <functional>
#include <json/json.h>
#include <time.h>
#include <cstdint>
#include <chrono>
#include <vector>
#include <Database/DatabaseAPI.hpp>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <Logger/Logger.hpp>
#include <future>
#include <miniz/miniz.h>
#include <sstream>

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

#define DEEPFORGE_TOOLSET_VERSION "0.1"
#define DEEPFORGE_TOOLSET_CHANNEL "stable"
#define NameProgram "InstallerDeepForge-Toolset"
#define __version__ "0.1"
#define __channel__ "stable"
#define APPINSTALLER_DB_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/AppInstaller.db"
#define DB_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db"
#define Locale_RU_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/locale_ru.json"
#define Locale_EN_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/locale_en.json"
#define RELEASE_MODE 1
#define DEBUG_MODE 0
#define MODE RELEASE_MODE

#if defined(__linux__)
#define URL_DESKTOP_SYMLINK "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/DeepForgeToolset.desktop"
#define NameVersionTable "LinuxVersions"
#define UpdateManagerTable "UpdateManager_Linux"
#define PATHMAN_AMD64_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-linux-amd64"
#define PATHMAN_ARM64_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-linux-armv8"
#define SERVICE_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/DeepForge-UpdateManager.service"
#define PYTHON_SCRIPT_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/InstallPackages.py"
#define OS_NAME "Linux"

#elif __APPLE__

#define OS_NAME "macOS"
#define UpdateManagerTable "UpdateManager_macOS"
#define NameVersionTable "macOSVersions"
#define PATHMAN_AMD64_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.6.0-darwin-amd64_v2.zip"
#define PATHMAN_ARM64_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.6.0-darwin-arm64.zip"
#define SHELL_SCRIPT_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/InstallPackages.sh"
#define UPDATE_MANAGER_PLIST_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/com.DeepForge.UpdateManager.plist"
#define ADD_TO_STARTUP_SH_URL "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/AddToStartup.sh"

#elif _WIN32

#define OS_NAME "Windows"
#define UpdateManagerTable "UpdateManager_Windows"
#define NameVersionTable "WindowsVersions"
#define PathmanURL_AMD64 "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-windows-amd64.exe"
#define PathmanURL_ARM64 "https://github.com/DeepForge-Tech/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-windows-amd64.exe"

#endif

// Variables
// int type
int Percentage;
int TempPercentage = 0;
// int result;
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
double downloadSpeed;
// map type
std::unordered_map<int, std::string> EnumerateChannels;
std::unordered_map<std::string, std::string> Channels;

/* The `replaceAll` function is a utility function that replaces all occurrences of a substring `from` with another substring `to` in a given string `str`. */
std::string ReplaceAll(std::string str, const std::string &from, const std::string &to)
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
const std::string TrueVarious[4] = {"yes", "y", "1","да"};
std::string ProjectFolder = std::filesystem::current_path().generic_string();
std::string haveString = "";
std::string new_sentence;
std::string LangReadySet;
std::string InstallDelimiter = "========================================================";
std::string Language;
std::string SelectPackages;
std::string Answer;
// Boolean type
bool Install;

#if defined(__APPLE__)

char *UserFolder = getenv("HOME");
#if defined(_M_AMD64) || defined(__x86_64__)
std::string Architecture = "amd64";
#elif __arm__ || __aarch64__ || _M_ARM64
std::string Architecture = "arm64";
#endif
std::string OrganizationFolder;
std::string DesktopPath;
std::string ApplicationFolder;
std::string TempFolder;
std::string LocaleFolder;
std::string UpdateManagerFolder;
std::string DatabasePath;
std::string LogPath;
#elif __linux__

#if defined(_M_AMD64) || defined(__x86_64__)
std::string Architecture = "amd64";
#elif __arm__ || __aarch64__ || _M_ARM64
std::string Architecture = "arm64";
#endif
std::string NameDistribution;
std::string PackageManager;
char *UserFolder = getenv("HOME");
const std::string DesktopPath = std::string(UserFolder) + "/Desktop";
std::string OrganizationFolder = "/usr/bin/DeepForge";
std::string ApplicationFolder = OrganizationFolder + "/DeepForge-Toolset";
const std::string UpdateManagerFolder = OrganizationFolder + "/UpdateManager";
std::string TempFolder = ApplicationFolder + "/Temp";
std::string LocaleFolder = ApplicationFolder + "/locale";
std::string DatabasePath = TempFolder + "/Versions.db";
// const std::string NewUpdateManagerFolder = OrganizationFolder + "/UpdateManager";
#elif _WIN32

#if defined(_M_AMD64) || defined(__x86_64__)
std::string Architecture = "amd64";
#elif __arm__ || __aarch64__ || _M_ARM64
std::string Architecture = "arm64";
#endif
char *UserFolder = getenv("USERPROFILE");
const std::string OrganizationFolder = "C:\\ProgramData\\DeepForge";
const std::string DesktopPath = std::string(UserFolder) + "\\Desktop";
std::string ApplicationFolder = "C:\\ProgramData\\DeepForge\\DeepForge-Toolset";
std::string TempFolder = ApplicationFolder + "\\Temp";
std::string DatabasePath = TempFolder + "\\Versions.db";
std::string LocaleFolder = ApplicationFolder + "\\locale";
const std::string UpdateManagerFolder = OrganizationFolder + "\\UpdateManager";
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
        std::unordered_map<std::string, std::string> ApplicationColumns = {
            {"Name", "TEXT"},
            {"Version", "TEXT"},
            {"NameTable", "TEXT"}};
        std::unordered_map<std::string, std::string> ApplicationFields = {
            {"Name", "DeepForge-Toolset"},
            {"Version", version},
            {"NameTable", NameVersionTable}};
        std::string pathFile = UpdateManagerFolder + "/AppInformation.db";
        DB::Database AppInformationDB;
        int result;
        /* The bellow code is checking if a file exists at the specified path. If the file does not exist, it creates a new file and writes an empty string to it. Then, it opens a database connection using the file as the database path. It checks if a table named "Applications" exists in the database. If the table does not exist, it creates the table with the specified columns. Finally, it inserts values into the "Applications" table. */
        if (std::filesystem::exists(pathFile) == false)
        {
            std::ofstream file(pathFile);
            file << "";
            file.close();
        }
        AppInformationDB.open(&pathFile);
        /* The bellow code is creating a table called "Applications" in the AppInformationDB database using the CreateTable method. It then inserts values into the "Applications" table using the InsertValuesToTable method. The boolean variable "exists" is used to store the result of the CreateTable method, indicating whether the table creation was successful or not. The integer variable "result" is used to store the number of rows affected by the InsertValuesToTable method. */
        AppInformationDB.CreateTable("Applications", ApplicationColumns);
        bool existsValue = AppInformationDB.ExistValueInTable("Applications","Name", "DeepForge-Toolset");
        /* The code is checking if a table called "Applications" exists in the database. If the table does not exist (existsTable == -1), it inserts values into the table using the AppInformationDB.InsertValuesToTable() method. If the table does exist, it removes an application called "DeepForge-Toolset" from the table using the AppInformationDB.RemoveApplicationFromTable() method, and then inserts values into the table using the AppInformationDB.InsertValuesToTable() method. */
        if (!existsValue)
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
        std::string logText = "==> ❌ " + std::string(error.what());
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "WriteInformation()", error.what());
        std::cerr << logText << std::endl;
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
 * @param TotalTodownload The total size of the file to be downloaded in bytes.
 * @param Nowdownloaded The amount of data that has been downloaded so far.
 * @param TotalToUpload The total size of the data to be uploaded in bytes.
 * @param NowUploaded The parameter "NowUploaded" represents the amount of data that has been
 * uploaded so far. It is a double data type.
 *
 * @return an integer value of 0.
 */
int CallbackProgress(void *ptr, double TotalTodownload, double Nowdownloaded, double TotalToUpload, double NowUploaded)
{
    if (TotalTodownload <= 0.0)
    {
        return 0;
    }
    if (withProgress == true)
    {
        // double downloadSpeed;
        Percentage = static_cast<float>(Nowdownloaded) / static_cast<float>(TotalTodownload) * 100;
        /* The bellow code is checking if the `TempPercentage` is not equal to `Percentage` and is less
        than or equal to 100. If this condition is true, it retrieves the download speed using
        `curl_easy_getinfo` and updates a progress bar using the `progressbar.Update` function. It
        also updates some variables (`LastdownloadSpeed`, `LastSize`, `LastTotalSize`, and
        `TempPercentage`) with the current values. */
        if (TempPercentage != Percentage && TempPercentage <= 100)
        {
            progressbar.update(Nowdownloaded, TotalTodownload);
            LastSize = Nowdownloaded;
            LastTotalSize = TotalTodownload;
            TempPercentage = Percentage;
        }
    }
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

bool endsWith(const std::string& s, const std::string& suffix)
{
    return s.rfind(suffix) == (s.size() - suffix.size());
}


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
bool CheckAnswer(std::string &answer)
{
    bool status = false;
    for (int i = 0; i < TrueVarious->size(); i++)
    {
        answer = to_lower(answer);
        if (answer == TrueVarious[i] || answer.empty())
        {
            status = true;
            break;
        }
    }
    return status;
}

bool CheckStringInFile(const std::string& filename, const std::string& target) {
    std::fstream file(filename,std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(target) != std::string::npos) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

#endif