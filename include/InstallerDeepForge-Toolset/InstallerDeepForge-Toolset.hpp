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
#ifndef INSTALLERDEEPFORGE_TOOLSET_HPP_
#define INSTALLERDEEPFORGE_TOOLSET_HPP_
// Checking the name of the operating system and importing the necessary libraries for this system
#if defined(__linux__)
#include <Linux.cpp>
using namespace Linux;
#elif __APPLE__
#include <macOS.cpp>
using namespace macOS;
#elif _WIN32
#include <Windows.cpp>
using namespace Windows;
#endif

class Application : public Installer
{
public:
    void SetLanguage();
    void DownloadDependencies();
    void InstallUpdateManager();
    void InstallDeepForgeToolset(std::string channel);
    void ChangeUpdating();
    void CommandManager();
    // JSON file reading function with interface localization
    void ReadJSON(std::string language);
    Application()
    {
#if defined(_WIN32)
        // Set console code page to UTF-8 so console known how to interpret string data
        SetConsoleOutputCP(CP_UTF8);
        // Create temp folder
        MakeDirectory(TempFolder);
        MakeDirectory(LocaleDir);
#elif defined(__APPLE__)
        std::string Command;
        OrganizationFolder = string(UserFolder) + "/Library/Containers/DeepForge";
        ApplicationDir = NewOrganizationFolder + "/DeepForge-Toolset";
        TempFolder = NewApplicationFolder + "/Temp";
        UpdateManagerFolder = NewOrganizationFolder + "/UpdateManager";
        LocaleDir = NewApplicationFolder + "/locale";
        DB_PATH = NewTempFolder + "/Versions.db";
        Command = "sudo -s chmod 777 " + string(UserFolder) + "/Library/Containers/";
        system(Command.c_str());
        // Create temp folder
        MakeDirectory(NewTempFolder);
        MakeDirectory(LocaleDir);
        Command = "sudo -s chmod 777 " + OrganizationFolder;
        system(Command.c_str());
        Command = "sudo -s chmod 777 " + OrganizationFolder + "/*";
        system(Command.c_str());
        Command = "sudo -s chmod 777 " + ApplicationDir + "/*";
        system(Command.c_str());
#elif __linux__
        std::string Command;
        Command = "sudo -s chmod 777 /usr/bin/";
        system(Command.c_str());
        // Create temp folder
        MakeDirectory(TempFolder);
        MakeDirectory(LocaleDir);
        Command = "sudo -s chmod 777 " + OrganizationFolder;
        system(Command.c_str());
        Command = "sudo -s chmod 777 " + OrganizationFolder + "/*";
        system(Command.c_str());
        Command = "sudo -s chmod 777 " + ApplicationDir + "/*";
        system(Command.c_str());
        // mkdir(OrganizationFolder.c_str(),0777);
        // mkdir(ApplicationDir.c_str(),0777);
        mkdir(TempFolder.c_str(), 0777);
        mkdir(LocaleDir.c_str(), 0777);
#endif
        DownloadDependencies();
        database.open(&DatabasePath);
        std::future<void> UploadInformation_async = std::async(std::launch::async, UploadInformation);
        UploadInformation_async.wait();
        std::cout << InstallDelimiter << std::endl;
    }

private:
};

#endif