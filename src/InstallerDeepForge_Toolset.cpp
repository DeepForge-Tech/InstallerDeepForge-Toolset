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
// Checking the name of the operating system and importing the necessary libraries for this system
#if defined(__linux__)
#include "include/Installer_Linux.hpp"
using namespace Linux;
#elif __APPLE__
#include "include/Installer_macOS.hpp"
using namespace macOS;
#elif _WIN32
#include "include/Installer_Windows.hpp"
using namespace Windows;
#endif

Installer installer;

string AllChannels[4] = {"stable\\latest", "stable", "beta", "beta\\latest"};

/**
 * The InstallUpdateManager function downloads and installs the latest version of the Update Manager
 * application.
 */
void Installer::InstallUpdateManager()
{
    try
    {
        string version;
        string UpdateManagerUrl;
        string name;
        string filename;
        string ArchivePath;
        string Command;
        string file_path;
        version = database.GetLatestVersion(UpdateManagerTable, "stable\\latest", "Version", Architecture);
        UpdateManagerUrl = database.GetApplicationURL(UpdateManagerTable, "stable\\latest", "Url", Architecture, version);
        cout << "==> Installing UpdateManager..." << endl;
        Download(UpdateManagerUrl, NewTempFolder);
        name = (UpdateManagerUrl.substr(UpdateManagerUrl.find_last_of("/")));
        ArchivePath = NewTempFolder + "/" + name.replace(name.find("/"), 1, "");
        MakeDirectory(NewUpdateManagerFolder);
        UnpackArchive(ArchivePath, NewUpdateManagerFolder);
        filesystem::remove(ArchivePath);
        file_path = NewUpdateManagerFolder + "/DeepForge-UpdateManager";
        cout << "==> ✅ UpdateManager " << version << " successfully installed" << endl;
        cout << InstallDelimiter << endl;
        AddToStartupSystem(file_path);
    }
    catch (exception &error)
    {
        string ErrorText = "==> ❌ " + string(error.what());
        logger.SendError(Architecture,"Empty",OS_NAME,"InstallUpdateManager()",error.what());
        cerr << ErrorText << endl;
    }
}

/**
 * The function `InstallDeepForgeToolset` installs the DeepForge Toolset by downloading it from a
 * specified channel, unpacking the archive, installing necessary libraries, creating a symlink, and
 * removing the archive.
 *
 * @param channel The "channel" parameter is a string that represents the channel from which the
 * DeepForge Toolset will be installed. It is used to retrieve the latest version and application URL
 * from the database.
 */
void Installer::InstallDeepForgeToolset(string channel)
{
    try
    {
        string version;
        string ApplicationURL;
        string name;
        string filename;
        string ArchivePath;
        string Command;
        string file_path;
        cout << InstallDelimiter << endl;
        cout << "==> Installing DeepForge Toolset..." << endl;
        version = database.GetLatestVersion(NameVersionTable, channel, "Version", Architecture);
        ApplicationURL = database.GetApplicationURL(NameVersionTable, channel, "Url", Architecture, version);
        Download(ApplicationURL, NewTempFolder);
        name = (ApplicationURL.substr(ApplicationURL.find_last_of("/")));
        ArchivePath = NewTempFolder + "/" + name.replace(name.find("/"), 1, "");
        MakeDirectory(NewApplicationFolder);
        UnpackArchive(ArchivePath, NewApplicationFolder);
        #if defined(__linux__)
            InstallLibraries();
        #elif __APPLE__
            InstallLibraries();
        #endif
        file_path = NewApplicationFolder + "/DeepForgeToolset";
        CreateSymlink("DeepForgeToolset", file_path);
        filesystem::remove(ArchivePath);
        cout << "==> ✅ DeepForge Toolset " << version << " successfully installed" << endl;
        cout << InstallDelimiter << endl;
        InstallUpdateManager();

    }
    catch (exception &error)
    {
        string ErrorText = "==> ❌ " + string(error.what());
        logger.SendError(Architecture,"Empty",OS_NAME,"InstallDeepForgeToolset()",error.what());
        cerr << ErrorText << endl;
    }
}

/**
 * The CommandManager function allows the user to select a version of the DeepForge Toolset and
 * installs it.
 */
void Installer::CommandManager()
{
    try
    {
        map<int, string> EnumerateChannels;
        map<string, string> Channels = database.GetAllVersionsFromDB(NameVersionTable, "Channel", Architecture);
        int size = (sizeof(AllChannels) / sizeof(AllChannels[0]));
        int n = 1;
        int defaultChannel = 1;
        /* The code is iterating over the `AllChannels` array and checking if each channel exists in
        the `Channels` map. If a channel exists, it prints the channel number and name, and inserts
        the channel into the `EnumerateChannels` map. If the channel is "stable\latest", it sets the
        `defaultChannel` variable to the current channel number. */
        for (int i = 0; i < size; i++)
        {

            if (Channels.find(AllChannels[i]) != Channels.end())
            {
                cout << n << ". " << AllChannels[i] << endl;
                EnumerateChannels.insert(pair<int, string>(n, AllChannels[i]));
                if (AllChannels[i] == "stable\\latest")
                {
                    defaultChannel = n;
                }
                n++;
            }
        }
        cout << "Change version of DeepForge Toolset (default - " << defaultChannel << "):";
        getline(cin, Answer);
        /* This code block is responsible for handling user input to select a version of the DeepForge
        Toolset to install. */
        if (Answer.empty())
        {
            installer.InstallDeepForgeToolset(EnumerateChannels[defaultChannel]);
        }
        else
        {
            int TempAnswer = stoi(Answer);
            if (EnumerateChannels.find(TempAnswer) != EnumerateChannels.end())
            {
                installer.InstallDeepForgeToolset(EnumerateChannels[TempAnswer]);
            }
            else
            {
                CommandManager();
            }
        }
    }
    catch (exception &error)
    {
        CommandManager();
    }
}

int main()
{
    installer.CommandManager();
    return 0;
}