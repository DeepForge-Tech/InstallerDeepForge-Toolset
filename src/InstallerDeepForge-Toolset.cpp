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
#include <InstallerDeepForge-Toolset/InstallerDeepForge-Toolset.hpp>

void Application::SetLanguage()
{
    std::string NumLang;
    LanguageChoices UserChoice;

    std::cout << "1. Russian" << std::endl;
    std::cout << "2. English" << std::endl;
    std::cout << "==> Choose language (default - 1): ";
    std::getline(std::cin, NumLang);

    if (NumLang.empty())
    {
        UserChoice = LanguageChoices::RUSSIAN;
    }
    else
    {
        UserChoice = static_cast<LanguageChoices>(stoi(NumLang) - 1);
    }

    switch (UserChoice)
    {
    case LanguageChoices::RUSSIAN:
        Language = "Russian";
        ReadJSON("Russian");
        break;
    case LanguageChoices::ENGLISH:
        Language = "English";
        ReadJSON("English");
        break;
    default:
        std::cout << InstallDelimiter << std::endl;
        std::cout << "Invalid choice, please choose again." << std::endl;
        std::cout << InstallDelimiter << std::endl;
        SetLanguage();
        break;
    }
    std::cout << InstallDelimiter << std::endl;
}
// JSON file reading function with interface localization
void Application::ReadJSON(std::string language)
{
    try
    {
        if (language == "Russian")
        {
            std::string LocalePath = LocaleFolder + "/locale_ru.json";
            std::ifstream file(LocalePath);
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
            std::string LocalePath = LocaleFolder + "/locale_en.json";
            std::ifstream file(LocalePath);
            // File open check
            if (file.is_open())
            {
                // Dictionary entry with translation
                file >> translate;
                file.close();
            }
        }
    }
    catch (std::exception &error)
    {
        // Error output
        std::string logText = "Function: ReadJSON()." + std::string(error.what());
        logger.writeLog("Error", logText);
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "ReadJSON", error.what());
    }
}
void Application::DownloadDependencies()
{
    std::string Locales[2] = {Locale_RU_URL, Locale_EN_URL};
    for (int i = 0; i < (sizeof(Locales) / sizeof(Locales[0])); i++)
    {
        Download(Locales[i], LocaleFolder, false);
    }
    SetLanguage();
    std::cout << translate["DownloadingDatabase"].asString() << std::endl;
    // download database Versions.db
    Download(DB_URL, TempFolder, true);
    std::cout << translate["DatabaseDownloaded"].asCString() << std::endl;
}

/**
 * The InstallUpdateManager function downloads and installs the latest version of the Update Manager
 * application.
 */
void Application::InstallUpdateManager()
{
    try
    {
        std::string latestVersion;
        std::string UpdateManagerUrl;
        std::string name;
        std::string filename;
        std::string archivePath;
        std::string command;
        std::string file_path;
        latestVersion = database.GetLatestVersion(UpdateManagerTable, "stable", "Version", Architecture);
        if (latestVersion != "")
        {
            UpdateManagerUrl = database.GetApplicationURL(UpdateManagerTable, "stable", "Url", Architecture, latestVersion);
            std::cout << translate["Installing"].asString() << " UpdateManager..." << std::endl;
            Download(UpdateManagerUrl, TempFolder, true);
            name = (UpdateManagerUrl.substr(UpdateManagerUrl.find_last_of("/")));
            archivePath = TempFolder + "/" + name.replace(name.find("/"), 1, "");
            MakeDirectory(UpdateManagerFolder);
            UnpackArchive(archivePath,UpdateManagerFolder);
#if defined(__linux__) || defined(__APPLE__)
            command = "sudo -s chmod 777 " + UpdateManagerFolder + "/*";
            system(command.c_str());
#endif
            std::filesystem::remove(archivePath);
            file_path = UpdateManagerFolder + "/UpdateManager";
            std::cout << "==> ✅ UpdateManager " << latestVersion << " " << translate["Installed"].asString() << std::endl;
            std::cout << InstallDelimiter << std::endl;
#if defined(_WIN32)
            AddToStartupSystem(file_path);
#else
            AddToStartupSystem();
#endif
            WriteInformation(DEEPFORGE_TOOLSET_VERSION);
        }
    }
    catch (std::exception &error)
    {
        std::string logText = "==> ❌ " + std::string(error.what());
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "InstallUpdateManager()", error.what());
        std::cerr << logText << std::endl;
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
void Application::InstallDeepForgeToolset(std::string channel)
{
    try
    {
        std::string ApplicationURL;
        std::string name;
        std::string filename;
        std::string archivePath;
        std::string command;
        std::string file_path;
        std::cout << InstallDelimiter << std::endl;
        std::cout << translate["Installing"].asString() << " DeepForge-Toolset..." << std::endl;
        ApplicationURL = database.GetApplicationURL(NameVersionTable, channel, "Url", Architecture, DEEPFORGE_TOOLSET_VERSION);
        Download(ApplicationURL, TempFolder, true);
        name = (ApplicationURL.substr(ApplicationURL.find_last_of("/")));
        archivePath = TempFolder + "/" + name.replace(name.find("/"), 1, "");
        MakeDirectory(ApplicationFolder);
        UnpackArchive(archivePath, ApplicationFolder);
        #if defined(__linux__)
                InstallLibraries();
        #elif __APPLE__
                InstallLibraries();
        #endif
        file_path = ApplicationFolder + "/DeepForge-Toolset";
        #if defined(_WIN32) || defined(__APPLE__)
        CreateSymlink("DeepForge-Toolset", file_path);
        #endif
        std::filesystem::remove(archivePath);
        std::cout << "==> ✅ DeepForge-Toolset " << DEEPFORGE_TOOLSET_VERSION << " " << translate["Installed"].asString() << std::endl;
        AddPath();
        std::cout << InstallDelimiter << std::endl;
        if (Updating == true)
        {
            InstallUpdateManager();
        }
    }
    catch (std::exception &error)
    {
        std::string logText = "==> ❌ " + std::string(error.what());
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "InstallDeepForgeToolset()", error.what());
        std::cerr << logText << std::endl;
    }
}

void Application::ChangeUpdating()
{
    std::cout << translate["ChangeUpdating"].asString();
    getline(std::cin, Answer);
    if (Answer.empty())
    {
        Updating = true;
    }
    else
    {
        Updating = CheckAnswer(Answer);
    }
}

void Application::CommandManager()
{
    try
    {
        ChangeUpdating();
        InstallDeepForgeToolset(DEEPFORGE_TOOLSET_CHANNEL);
        std::cout << translate["Reboot"].asString();
        getline(std::cin, Answer);
        if (Answer.empty() || CheckAnswer(Answer) == true)
        {
            RebootSystem();
        }
    }
    catch (std::exception &error)
    {
        CommandManager();
    }
}

int main(int argc, char **argv)
{
    #if defined(__APPLE__)
        std::filesystem::path current_dir = argv[0];
        std::filesystem::current_path(current_dir.parent_path().generic_string());
    #endif
    Application app;
    app.CommandManager();
    return 0;
}