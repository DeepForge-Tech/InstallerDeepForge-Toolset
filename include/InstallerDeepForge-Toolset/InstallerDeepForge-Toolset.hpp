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
#elif __linux__
        std::string Command;
        Command = "sudo -s chmod 777 /usr/bin/";
        std::cout << Command << std::endl;
        system(Command.c_str());
        // Create temp folder
        MakeDirectory(TempFolder);
        MakeDirectory(LocaleDir);
        // Command = "sudo -s chmod 777 " + OrganizationFolder;
        // system(Command.c_str());
        // Command = "sudo -s chmod 777 " + OrganizationFolder + "/*";
        // system(Command.c_str());
        // Command = "sudo -s chmod 777 " + ApplicationDir + "/*";
        // system(Command.c_str());
#endif
        std::cout << "1" << std::endl;
        DownloadDependencies();
        std::cout << "2" << std::endl;
        database.open(&DatabasePath);
        std::cout << "3" << std::endl;
        std::future<void> UploadInformation_async = std::async(std::launch::async, UploadInformation);
        UploadInformation_async.wait();
        std::cout << InstallDelimiter << std::endl;
    }

private:
};

#endif