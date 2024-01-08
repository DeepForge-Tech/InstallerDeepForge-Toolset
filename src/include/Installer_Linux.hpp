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
#include "../DatabaseConnect.hpp"
#include <map>
#include "Logger.cpp"
#include "json/json.h"
#include <string>
#include <vector>
#include "zip/zip.h"
#include <cstring>
#include <fstream>
#include <future>

#define DEEPFORGE_TOOLSET_VERSION "0.1"
#define URL_DESKTOP_SYMLINK "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/DeepForgeToolset.desktop"
#define DB_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db"
#define Locale_RU_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/locale_ru.json"
#define Locale_EN_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/locale_en.json"
#define NameVersionTable "DeepForgeToolset_Linux"
#define UpdateManagerTable "UpdateManager_Linux"
#define PATHMAN_AMD64_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-linux-amd64"
#define PATHMAN_ARM64_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/pathman-v0.5.2-linux-armv8"
#define SERVICE_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/DeepForge-UpdateManager.service"
#define SHELL_SCRIPT_URL "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/InstallLibraries_Linux.sh"
#define OS_NAME "Linux"

using namespace std;
using namespace DB;
using namespace Bar;

namespace Linux
{
    // int type
    int result;
    int Percentage;
    int TempPercentage = 0;
    int defaultChannel = 1;
    // boold type
    bool Updating = true;
    bool withProgress = true;
    // float type
    float LastSize;
    float LastTotalSize;
    float DownloadSpeed;
    // map type
    map<int, string> EnumerateChannels;
    map<string, string> Channels;
    // string type
    const string NewOrganizationFolder = "/usr/bin/DeepForge";
    const string NewApplicationFolder = NewOrganizationFolder + "/DeepForge-Toolset";
    const string NewUpdateManagerFolder = NewOrganizationFolder + "/UpdateManager";
    const string NewTempFolder = NewApplicationFolder + "/Temp";
    const string LocaleDir = NewTempFolder + "/locale";
    filesystem::path ProjectDir = filesystem::current_path().generic_string();
    string DB_PATH = NewTempFolder + "/Versions.db";
    const string TrueVarious[3] = {"yes", "y", "1"};
    string Answer;
    string AllChannels[2] = {"stable", "beta"};
    string InstallDelimiter = "========================================================";
    #if defined(__x86_64__)
        string Architecture = "amd64";
    #elif __arm__ || __aarch64__ || _M_ARM64
        string Architecture = "arm64";
    #endif
    Json::Value translate;
    // init classes
    CURL *curl = curl_easy_init();
    Logger logger;
    CURLcode res;
    ProgressBar_v1 progressbar;
    Database database;

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
        if (withProgress == true)
        {
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
    void UploadInformation()
    {
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
                EnumerateChannels.insert(pair<int, string>(n, AllChannels[i]));
                if (AllChannels[i] == "stable")
                {
                    defaultChannel = n;
                }
                n++;
            }
        }
    }
    void DownloadLocales()
    {
        try
        {
            withProgress = false;
            string Locales[2] = {Locale_RU_URL,Locale_EN_URL};
            for (int i = 0;i < (sizeof(Locales) / sizeof(Locales[0]));i++)
            {
                string url = Locales[i];
                // Get name of file from url
                string name = (url.substr(url.find_last_of("/")));
                string filename = LocaleDir + "/" + name.replace(name.find("/"), 1, "");
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
                            throw domain_error(translate["CURLE_COULDNT_CONNECT"].asCString());
                        case CURLE_COULDNT_RESOLVE_HOST:
                            throw domain_error(translate["CURLE_COULDNT_RESOLVE_HOST"].asCString());
                        case CURLE_COULDNT_RESOLVE_PROXY:
                            throw domain_error(translate["CURLE_COULDNT_RESOLVE_PROXY"].asCString());
                        case CURLE_UNSUPPORTED_PROTOCOL:
                            throw domain_error(translate["CURLE_UNSUPPORTED_PROTOCOL"].asCString());
                        case CURLE_SSL_CONNECT_ERROR:
                            throw domain_error(translate["CURLE_SSL_CONNECT_ERROR"].asCString());
                    }
                }
                curl_easy_cleanup(curl);
                fclose(file);
            }
        }
        catch (exception &error)
        {
            string ErrorText = "==> ❌ " + string(error.what());
            logger.SendError(Architecture, "Empty", OS_NAME, "Download()", error.what());
            cerr << ErrorText << endl;
        }
    }
    // Main class
    class Installer
    {
    public:
        Installer()
        {
            string Command = "sudo -s chmod 777 /usr/bin/";
            system(Command.c_str());
            // Create temp folder
            MakeDirectory(NewTempFolder);
            MakeDirectory(LocaleDir);
            DownloadLocales();
            ChangeLanguage();
            cout << "==> " << translate["DownloadingDatabase"].asString() << endl;
            // Download database Versions.db
            Download(DB_URL, NewTempFolder, true);
            database.open(&DB_PATH);
            std::future<void> UploadInformation_async = std::async(std::launch::async, UploadInformation);
            UploadInformation_async.wait();
            cout << "==> " << translate["DatabaseDownloaded"].asCString() << endl;
            cout << InstallDelimiter << endl;
        }
        void CommandManager();
        void AddToPATH()
        {
            string Command;
            #if defined(__x86_64__)
                Command = "cd " + NewTempFolder +  "&& sudo -s chmod +x pathman-v0.5.2-linux-amd64 &&  sudo -s ./pathman-v0.5.2-linux-amd64 add /bin/";
                Download(PATHMAN_AMD64_URL,NewTempFolder,false);
            #elif __arm__
                 "cd " + NewTempFolder +  "&& sudo -s chmod +x pathman-v0.5.2-linux-armv8 &&  sudo -s ./pathman-v0.5.2-linux-armv8 add /bin/";
                Download(PATHMAN_ARM64_URL,NewTempFolder,false);
            #endif
            system(Command.c_str());
        }
        void InstallUpdateManager();
        void InstallDeepForgeToolset(string channel);
        void ChangeUpdating();
        void ChangeLanguage()
        {
            string NumLang;
            cout << "1. Russian" << endl;
            cout << "2. English" << endl;
            cout << "==> Choose language (default - 1):";
            getline(cin, NumLang);
            cout << InstallDelimiter << endl;
            if (NumLang == "1" || NumLang.empty())
            {
                ReadJSON("Russian");
            }
            else if (NumLang == "2")
            {
                ReadJSON("English");
            }
            // If the user enters a non-digit, then the method is called again
            else
            {
                ChangeLanguage();
            }
        }
        // JSON file reading function with interface localization
        void ReadJSON(string language)
        {
            try
            {
                if (language == "Russian")
                {
                    string LocalePath = LocaleDir + "/locale_ru.json";
                    ifstream file(LocalePath);
                    // File open check
                    if (file.is_open())
                    {
                        // Dictionary entry with translation
                        file >> translate;
                        file.close();
                    }
                }
                else if (language == "English")
                {
                    string LocalePath = LocaleDir + "/locale_en.json";
                    ifstream file(LocalePath);
                    // File open check
                    if (file.is_open())
                    {
                        // Dictionary entry with translation
                        file >> translate;
                        file.close();
                    }
                }
            }
            catch (exception &error)
            {
                // Error output
                logger.WriteError("Function: ReadJSON");
                logger.WriteError(error.what());
                logger.SendError(Architecture,"Empty",OS_NAME,"ReadJSON",error.what());
            }
        }
    private:
        void CreateSymlink(string nameSymlink, string filePath)
        {
            char *UserFolder = getenv("HOME");
            string symlinkPath = string(UserFolder) + "/Desktop/" + nameSymlink;
            string Command = "sudo ln -s " + filePath + " " + symlinkPath;
            system(Command.c_str());
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
            try
            {
                MakeDirectory(path_to);
                int err;
                struct zip *zip = zip_open(path_from.c_str(), ZIP_RDONLY, &err);
                if (zip == nullptr)
                {
                    string ErrorText = "==> ❌" + translate["CannotOpenArchive"].asString() + path_from;
                    throw runtime_error(ErrorText);
                }

                int num_entries = zip_get_num_entries(zip, 0);
                for (int i = 0; i < num_entries; ++i)
                {
                    zip_stat_t zip_stat;
                    zip_stat_init(&zip_stat);
                    int err = zip_stat_index(zip, i, 0, &zip_stat);
                    if (err != 0)
                    {
                        zip_close(zip);
                    }

                    string file_name = zip_stat.name;
                    string full_path = path_to + "/" + file_name;
                    filesystem::path file_dir(full_path);
                    MakeDirectory(file_dir.remove_filename().string());

                    struct zip_file *zip_file = zip_fopen_index(zip, i, 0);
                    if (zip_file == nullptr)
                    {
                        string ErrorText = "==> ❌" + translate["CannotOpenFile"].asString() + file_name;
                        zip_close(zip);
                        throw runtime_error(ErrorText);
                    }

                    if(filesystem::is_directory(full_path) == false)
                    {
                        ofstream out_file(full_path,ios::binary);
                        if (!out_file.is_open())
                        {
                            string ErrorText = "==> ❌" + translate["CannotWriteFile"].asString() + full_path;
                            zip_fclose(zip_file);
                            zip_close(zip);
                            throw runtime_error(ErrorText);
                        }
                        vector<char> buffer(zip_stat.size);
                        zip_fread(zip_file, buffer.data(), buffer.size());
                        out_file.write(buffer.data(), buffer.size());
                        out_file.close();
                    }
                    

                    zip_fclose(zip_file);
                }

                zip_close(zip);
            }
            catch (exception& error)
            {
                logger.SendError(Architecture, "Empty", OS_NAME, "UnpackArchive()", error.what());
                cerr << error.what() << endl;
            }
        }
        void WriteInformation(string version)
        {
            try
            {
                string NameTable = "DeepForgeToolset_" + string(OS_NAME);
                map<string, string> ApplicationColumns = {
                    {"Name", "TEXT"},
                    {"Version", "TEXT"},
                    {"NameTable","TEXT"}
                };
                map<string, string> ApplicationFields = {
                    {"Name", "DeepForge-Toolset"},
                    {"Version", version},
                    {"NameTable",NameTable}
                };
                string pathFile = NewUpdateManagerFolder + "/AppInformation.db";
                Database AppInformationDB;
                /* The bellow code is checking if a file exists at the specified path. If the file does not exist, it creates a new file and writes an empty string to it. Then, it opens a database connection using the file as the database path. It checks if a table named "Applications" exists in the database. If the table does not exist, it creates the table with the specified columns. Finally, it inserts values into the "Applications" table. */
                if (filesystem::exists(pathFile) == false)
                {
                    ofstream file(pathFile);
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
                    int result = AppInformationDB.InsertValuesToTable("Applications", ApplicationFields);
                }
                else
                {
                    AppInformationDB.RemoveApplicationFromTable("Applications", "DeepForge-Toolset");
                    int result = AppInformationDB.InsertValuesToTable("Applications", ApplicationFields);
                }
            }
            catch (exception &error)
            {
                // Error output
                logger.SendError(Architecture, "Empty", OS_NAME, "WriteInformation", error.what());
                cerr << "❌ " << error.what() << endl;
            }
        }
        void AddToStartupSystem()
        {
            string ServicePath = NewTempFolder + "/DeepForge-UpdateManager.service";
            string Command = "sudo mv " + ServicePath + " /etc/systemd/system/DeepForge-UpdateManager.service && sudo chmod 644 /etc/systemd/system/DeepForge-UpdateManager.service && sudo systemctl enable /etc/systemd/system/DeepForge-UpdateManager.service";
            Download(SERVICE_URL,NewTempFolder,false);
            system(Command.c_str());
        }
        /* The `InstallLibraries()` function is responsible for downloading and executing a shell script that installs additional libraries or dependencies required by the DeepForge Toolset. */
        void InstallLibraries()
        {
            string ShellScriptPath;
            string Command;
            Download(SHELL_SCRIPT_URL, NewTempFolder,false);
            // name = (ShellScript_URL.substr(ShellScript_URL.find_last_of("/")));
            ShellScriptPath = NewTempFolder + "/" + "InstallLibraries_Linux.sh";
            Command = "sudo bash " + ShellScriptPath;
            system(Command.c_str());
        }
        /* The `RebootSystem()` function is responsible for rebooting the system. It uses the `system()` function to execute the command `sudo shutdown -r now`, which instructs the system to restart immediately. */
        void RebootSystem()
        {
            system("sudo shutdown -r now");
        }
        // Method of make string to lower
        string to_lower(string sentence)
        {
            string new_sentence = "";
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
        bool CheckAnswer(string answer)
        {
            bool status = false;
            string Answer = to_lower(answer);
            // string Answer = answer;
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

        void Download(string url, string dir,bool Progress)
        {
            try {
                withProgress = Progress;
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
                            throw domain_error(translate["CURLE_COULDNT_CONNECT"].asCString());
                        case CURLE_COULDNT_RESOLVE_HOST:
                            throw domain_error(translate["CURLE_COULDNT_RESOLVE_HOST"].asCString());
                        case CURLE_COULDNT_RESOLVE_PROXY:
                            throw domain_error(translate["CURLE_COULDNT_RESOLVE_PROXY"].asCString());
                        case CURLE_UNSUPPORTED_PROTOCOL:
                            throw domain_error(translate["CURLE_UNSUPPORTED_PROTOCOL"].asCString());
                        case CURLE_SSL_CONNECT_ERROR:
                            throw domain_error(translate["CURLE_SSL_CONNECT_ERROR"].asCString());
                    }
                }
                curl_easy_cleanup(curl);
                fclose(file);
                // If the progress bar is not completely filled in, then paint over manually
                if (Progress == true)
                {
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
                }
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