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
#include <conio.h>
#include <Windows.h>
#include "Progressbar.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../DatabaseConnect.cpp"
#include <map>
#include <zipper/unzipper.h>
#include <fstream>
#include <cctype>
#include "Logger.cpp"

#define OS_NAME "Windows"

using namespace std;
using namespace DB;
using namespace Bar;
using namespace zipper;

namespace Windows
{
    // int type
    int result;
    int Percentage;
    int TempPercentage = 0;
    // float type
    double LastSize;
    double LastTotalSize;
    double DownloadSpeed;
    // string type
    string Answer;
    const string NewOrganizationFolder = "C:\\ProgramData\\DeepForge";
    const string NewApplicationFolder = "C:\\ProgramData\\DeepForge\\DeepForge-Toolset";
    const string NewUpdateManagerFolder = NewOrganizationFolder + "\\UpdateManager";
    const string NewTempFolder = NewApplicationFolder + "\\Temp";
    const string DB_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db";
    std::filesystem::path ProjectDir = std::filesystem::current_path().generic_string();
    string DB_PATH = NewTempFolder + "\\Versions.db";
    const string NameVersionTable = "WindowsVersions";
    const string UpdateManagerTable = "UpdateManager_Windows";
    const string TrueVarious[3] = {"yes", "y", "1"};
    string InstallDelimiter = "========================================================";
    #if defined(__x86_64__)
        string Architecture = "amd64";
    #elif __arm__
        string Architecture = "arm64";
    #endif
    // init classes
    Logger logger;
    ProgressBar_v1 progressbar;
    Database database;

    class WriteData : public IBindStatusCallback
    {
    public:
        HRESULT __stdcall QueryInterface(const IID &, void **)
        {
            return E_NOINTERFACE;
        }
        ULONG STDMETHODCALLTYPE AddRef(void)
        {
            return 1;
        }
        ULONG STDMETHODCALLTYPE Release(void)
        {
            return 1;
        }
        HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, IBinding *pib)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE GetPriority(LONG *pnPriority)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
        {
            return S_OK;
        }
        virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, IUnknown *punk)
        {
            return E_NOTIMPL;
        }

        virtual HRESULT __stdcall OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
        {

            Percentage = static_cast<float>(ulProgress) / static_cast<float>(ulProgressMax) * 100;
            if (TempPercentage != Percentage && TempPercentage <= 100)
            {
                progressbar.Update((double)(ulProgress), (double)(ulProgressMax));
                LastSize = (double)(ulProgress);
                LastTotalSize = (double)(ulProgressMax);
                TempPercentage = Percentage;
            }
            return S_OK;
        }
    };
    // Main class
    class Installer
    {
    public:
        Installer()
        {
            // Set console code page to UTF-8 so console known how to interpret string data
            SetConsoleOutputCP(CP_UTF8);
            // Create temp folder
            MakeDirectory(NewTempFolder);
            cout << InstallDelimiter << endl;
            cout << "Downloading database..." << endl;
            // Download database Versions.db
            Download(DB_URL, NewTempFolder);
            cout << "Database successfully downloaded." << endl;
            cout << InstallDelimiter << endl;
            database.open(&DB_PATH);
        }
        void CommandManager();
        void InstallUpdateManager();
        void InstallDeepForgeToolset(string channel);

    private:
        void Download(string url, string dir)
        {
            try
            {
                // Class for write data on windows
                WriteData writer;
                // Get name of file from url
                string name = (url.substr(url.find_last_of("/")));
                string filename = dir + "/" + name.replace(name.find("/"), 1, "");
                // Call method for download file
                HRESULT Download = URLDownloadToFile(NULL, url.c_str(), filename.c_str(), 0, static_cast<IBindStatusCallback *>(&writer));
                switch (Download)
                {
                    case -2146697211:
                        throw domain_error("No internet connection");
                    case -2147467260:
                        throw domain_error("Connection reset");
                }
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
            }
            catch (exception& error)
            {
                string ErrorText = "==> ❌ " + string(error.what());
                logger.SendError(Architecture,"Empty",OS_NAME,"Download()",error.what());
                cerr << ErrorText << endl;
            }
        }
        // Method for create symlink on desktop
        void CreateSymlink(string nameSymlink, string filePath)
        {
            nameSymlink = nameSymlink + ".exe";
            filePath = filePath + ".exe";
            char *UserFolder = getenv("USERPROFILE");
            string symlinkPath = string(UserFolder) + "\\Desktop\\" + nameSymlink;
            CreateHardLinkA(symlinkPath.c_str(), filePath.c_str(), NULL);
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
        // Method to iterate through creating folders
        void MakeDirectory(string dir)
        {
            string currentDir;
            string fullPath = "";
            string delimiter = "\\";
            size_t pos = 0;
            while ((pos = dir.find(delimiter)) != string::npos)
            {
                currentDir = dir.substr(0, pos);
                if (fullPath != "")
                {
                    fullPath = fullPath + "\\" + currentDir;
                    if (filesystem::exists(fullPath) == false)
                    {
                        filesystem::create_directory(fullPath);
                    }
                }
                else
                {
                    fullPath = currentDir + "\\";
                }
                dir.erase(0, pos + delimiter.length());
            }
            fullPath = fullPath + "\\" + dir;
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
            filePath = filePath + ".exe";
            HKEY hKey;
            const char* czStartName = "DeepForge-UpdateManager";
            // const char* czExePath   = "C:\\Users\\user\\AppData\\Roaming\\Microsoft\\Windows\\DeepForgeToolset.exe";

            LONG lnRes = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                        0 , KEY_WRITE,
                                        &hKey);
            if( ERROR_SUCCESS == lnRes )
            {
                lnRes = RegSetValueEx(  hKey,
                                        czStartName,
                                        0,
                                        REG_SZ,
                                        (unsigned char*)filePath.c_str(),
                                        strlen(filePath.c_str()) );
            }

            RegCloseKey(hKey);
        }
        void RebootSystem()
        {
            system("shutdown -r -t 0");
        }
        // Function for check of answer
        bool CheckAnswer(string answer)
        {
            bool status;
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
    };
}