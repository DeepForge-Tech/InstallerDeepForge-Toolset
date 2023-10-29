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
    Copyright (c) 2023 DeepForge Technology
    ============================================================================
    Organization: DeepForge Technology
    ============================================================================
    Author: Blackflame576
    ============================================================================
    Created: 4 Juny 2023
    ============================================================================
*/
#include <filesystem>
#include <iostream>
#include "Progressbar.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "../DatabaseConnect.cpp"
#include <map>
#include "zipper/unzipper.h"
#include "Logger.cpp"

#define OS_NAME "macOS"

using namespace std;
using namespace DB;
using namespace Bar;
using namespace zipper;

namespace macOS
{
    // int type
    int result;
    int Percentage;
    int TempPercentage = 0;
    // float type
    float LastSize;
    float LastTotalSize;
    float DownloadSpeed;
    // init classes
    Logger logger;
    ProgressBar_v1 progressbar;
    CURL *curl = curl_easy_init();
    CURLcode res;
    Database database;
    // string type
    #if defined(__x86_64__)
        string Architecture = "amd64";
    #elif __arm__
        string Architecture = "arm64";
    #endif
    string Answer;
    string NewOrganizationFolder;
    string NewApplicationFolder = "";
    string NewTempFolder;
    string NewUpdateManagerFolder;
    const string ShellScript_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/InstallLibraries.sh";
    const string DB_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db";
    std::filesystem::path ProjectDir = std::filesystem::current_path().generic_string();
    string DB_PATH;
    string NameVersionTable = "macOSVersions";
    const string TrueVarious[3] = {"yes", "y", "1"};
    string InstallDelimiter = "========================================================";

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
    // Main class
    class Installer
    {
    public:
        Installer()
        {
            string Command;
            char *UserFolder = getenv("HOME");
            NewOrganizationFolder = string(UserFolder) + "/Library/Containers/DeepForge";
            NewApplicationFolder = NewOrganizationFolder + "/DeepForge-Toolset";
            NewTempFolder = NewApplicationFolder + "/Temp";
            NewUpdateManagerFolder = NewOrganizationFolder + "/UpdateManager";
            DB_PATH = NewTempFolder + "/Versions.db";
            Command = "sudo -s chmod 777 " + string(UserFolder) + "/Library/Containers/";
            system(Command.c_str());
            // Create temp folder
            MakeDirectory(NewTempFolder);
            cout << "Downloading database..." << endl;
            // Download database Versions.db
            Download(DB_URL, NewTempFolder);
            cout << "Database successfully downloaded." << endl;
            database.open(&DB_PATH);
        }
        void CommandManager();
        void InstallDeepForgeToolset(string channel);

    private:
        /* The `CreateSymlink` function is creating a symbolic link (symlink) in the `/Applications` directory. It takes two parameters: `nameSymlink` which is the name of the symlink, and `filePath` which is the path to the file or directory that the symlink will point to.*/
        void CreateSymlink(string nameSymlink, string filePath)
        {
            // char *UserFolder = getenv("USER");
            string symlinkPath = "/Applications/" + nameSymlink;
            string Command = "sudo ln -s " + filePath + " " + nameSymlink;
            system(Command.c_str());
            // cout << symlinkPath << endl;
            // CreateHardLinkA(symlinkPath.c_str(), filePath.c_str(), NULL);
        }
        /*The `MakeDirectory` function is responsible for creating a directory (folder) in the file system.*/
        void MakeDirectory(string dir)
        {
            string currentDir;
            string fullPath = "";
            string delimiter = "/";
            size_t pos = 0;
            while ((pos = dir.find(delimiter)) != string::npos)
            {
                currentDir = dir.substr(0, pos);
                if (fullPath != "")
                {
                    fullPath = fullPath + "/" + currentDir;
                    if (filesystem::exists(fullPath) == false)
                    {
                        filesystem::create_directory(fullPath);
                    }
                }
                else
                {
                    fullPath = "/" + currentDir;
                }
                dir.erase(0, pos + delimiter.length());
            }
            fullPath = fullPath + "/" + dir;
            if (filesystem::exists(fullPath) == false)
            {
                filesystem::create_directory(fullPath);
            }
        }
        /*  The `UnpackArchive` function takes two parameters: `path_from` and `path_to`.
            It uses the `Unzipper` class to extract the contents of an archive file located at `path_from` and saves them to the directory specified by `path_to`.
            After extracting the contents, the function closes the `Unzipper` object.
        */
        void UnpackArchive(string path_from, string path_to)
        {
            Unzipper unzipper(path_from);
            unzipper.extract(path_to);
            unzipper.close();
        }
        void AddToStartupSystem(string filePath)
        {

        }
        /* The `InstallLibraries()` function is responsible for downloading and executing a shell script that installs additional libraries or dependencies required by the DeepForge Toolset. */
        void InstallLibraries()
        {
            string name;
            string ShellScriptPath;
            string Command;
            Download(ShellScript_URL, NewTempFolder);
            name = (ShellScript_URL.substr(ShellScript_URL.find_last_of("/")));
            ShellScriptPath = NewTempFolder + "/" + name.replace(name.find("/"), 1, "");
            Command = "bash " + ShellScriptPath;
            system(Command.c_str());
        }
        /* The `RebootSystem()` function is responsible for rebooting the system. It uses the `system()` function to execute the command `sudo shutdown -r now`, which instructs the system to restart immediately. */
        void RebootSystem()
        {
            system("sudo shutdown -r now");
        }
        // Function for check of answer
        bool CheckAnswer(string answer)
        {
            bool status;
            string Answer = answer;
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

        void Download(string url, string dir)
        {
            try {
                string name = (url.substr(url.find_last_of("/")));
                string filename = dir + "/" + name.replace(name.find("/"), 1, "");
                FILE *file = fopen(filename.c_str(), "wb");
                CURL *curl = curl_easy_init();
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
                curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, &CallbackProgress);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
                curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteData);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
                CURLcode response = curl_easy_perform(curl);
                if (response != CURLE_OK)
                {
                    switch (response)
                    {
                    case CURLE_COULDNT_CONNECT:
                        throw domain_error("Failed to connect to host or proxy.");
                    case CURLE_COULDNT_RESOLVE_HOST:
                        throw domain_error("Failed to resolve host. The given remote host was not allowed.");
                    case CURLE_COULDNT_RESOLVE_PROXY:
                        throw domain_error("Failed to resolve proxy. The given proxy host could not be resolved.");
                    case CURLE_UNSUPPORTED_PROTOCOL:
                        throw domain_error("Failed to connect to the site using this protocol.");
                    case CURLE_SSL_CONNECT_ERROR:
                        throw domain_error("The problem occurred during SSL/TLS handshake.");
                    }
                }
                curl_easy_cleanup(curl);
                fclose(file);
                // If the progress bar is not completely filled in, then paint over manually
                if (Process < 100 && Process != Percentage)
                {
                    for (int i = (Process - 1); i < 99; i++)
                    {
                        progressbar.Update(LastSize, LastTotalSize);
                    }
                }
                // Reset all variables and preferences
                progressbar.ResetAll();
                Percentage = 0;
                TempPercentage = 0;
                cout << InstallDelimiter << endl;
            }
            catch (exception& error)
            {
                string ErrorText = "==> ❌ " + string(error.what());
                logger.SendError(Architecture,"Empty",OS_NAME,"Download()",error.what());
                cerr << ErrorText << endl;
            }
        }
    };
}