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

using namespace std;
using namespace DB;
using namespace Bar;

namespace macOS
{
    int result;
    int Percentage;
    int TempPercentage = 0;
    string Architecture;
    float LastSize;
    float LastTotalSize;
    string Answer;
    const string NewApplicationFolder = "/usr/bin/DeepForge/DeepForge-Toolset";
    const string NewTempFolder = NewApplicationFolder + "\\Temp";
    ProgressBar_v1 progressbar;
    const string DB_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db";
    std::filesystem::path ProjectDir = std::filesystem::current_path().generic_string();
    string DB_PATH = NewTempFolder + "/Versions.db";
    string NameVersionTable = "macOSVersions";
    const string TrueVarious[3] = {"yes", "y", "1"};
    string InstallDelimiter = "========================================================";
    CURL *curl = curl_easy_init();
    CURLcode res;

    // Function for calc percentage of download progresss 
    int CallbackProgress(void *ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
    {
        if (TotalToDownload <= 0.0)
        {
            return 0;
        }

        Percentage = static_cast<float>(NowDownloaded) / static_cast<float>(TotalToDownload) * 100;
        if (TempPercentage != Percentage && TempPercentage <= 100)
        {
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &DownloadSpeed);
            if ((CURLE_OK == res) && (DownloadSpeed > 0.0))
            {
                // printf("Average download speed: %lu kbyte/sec.\n",
                //         (unsigned long)(DownloadSpeed / 1024));
                float Speed = (float)(DownloadSpeed / 1024);
                progressbar.Update(Speed);
                TempPercentage = Percentage;
            }
        }
        return 0;
    }

    // Function for write data from curl
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
            GetArchitectureOS();
        }
        void CommandManager();
        void InstallDeepForgeToolset(string channel);

    private:
        void CreateSymlink(string nameSymlink, string filePath)
        {
            char *UserFolder = getenv("USER");
            string symlinkPath = string(UserFolder) + "\\Desktop\\" + nameSymlink;
            string Command = "sudo ln -s " + filePath + " " + nameSymlink;
            system(Command.c_str());
            // cout << symlinkPath << endl;
            // CreateHardLinkA(symlinkPath.c_str(), filePath.c_str(), NULL);
        }
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
                    fullPath = "/" + currentDir + "/";
                }
                dir.erase(0, pos + delimiter.length());
            }
            fullPath = fullPath + "/" + dir;
            if (filesystem::exists(fullPath) == false)
            {
                filesystem::create_directory(fullPath);
            }
        }
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
        int Download(string url,string dir)
        {
            try
            {
                string name = (url.substr(url.find_last_of("/")));
                string filename = dir + "/" + name.replace(name.find("/"), 1, "");
                FILE *file = fopen(filename.c_str(), "wb");
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
                curl_easy_cleanup(curl);
                fclose(file);
                if (Process < 100)
                {
                    for (int i = (Process - 1); i < 99; i++)
                    {
                        progressbar.Update(0.0, LastSize, LastTotalSize);
                    }
                }
                // Reset all variables and preferences
                progressbar.ResetAll();
                return 200;
            }
            catch (exception &error)
            {
                return 502;
            }
        }
        // Method for getting architecture of OS
        void GetArchitectureOS()
        {
            #if defined(__x86_64__)
                Architecture = "amd64";
            #elif __arm__
                Architecture = "arm64";
            #endif
        }
    };
}