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
#include <InstallerDeepForge-Toolset/Windows.hpp>

/*  The `UnpackArchive` function takes two parameters: `path_from` and `path_to`.
            It uses the `Unzipper` class to extract the contents of an archive file located at `path_from` and saves them to the directory specified by `path_to`.
            After extracting the contents, the function closes the `Unzipper` object.
        */
void Windows::Installer::UnpackArchive(std::string path_from, std::string path_to)
{
    // std::string unpack_command = "tar -xf" + path_from + " --directory " + path_to;
    // system(unpack_command.c_str());
    try
    {
        MakeDirectory(path_to);

        mz_zip_archive zip_archive;
        memset(&zip_archive, 0, sizeof(zip_archive));

        mz_zip_reader_init_file(&zip_archive, path_from.c_str(), 0);

        for (int i = 0; i < mz_zip_reader_get_num_files(&zip_archive); i++)
        {
            mz_zip_archive_file_stat file_stat;
            mz_zip_reader_file_stat(&zip_archive, i, &file_stat);

            std::string output_path = path_to + "/" + file_stat.m_filename;
            std::filesystem::path path(output_path);
            std::filesystem::create_directories(path.parent_path());

            std::ofstream out(output_path, std::ios::binary);
            if (!out)
            {
                std::cerr << "Failed to create file: " << output_path << std::endl;
                continue;
            }

            void* fileData = mz_zip_reader_extract_to_heap(&zip_archive, file_stat.m_file_index, &file_stat.m_uncomp_size, 0); // You can adjust the flags parameter as needed
            if (!fileData)
            {
                std::cerr << "Failed to extract file: " << file_stat.m_filename << std::endl;
                continue;
            }

            out.write(static_cast<const char *>(fileData), file_stat.m_uncomp_size);
            mz_free(fileData);

            out.close();
        }

        mz_zip_reader_end(&zip_archive);
    }
    catch (std::exception &error)
    {
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "UnpackArchive()", error.what());
        std::cerr << error.what() << std::endl;
    }
}

void Windows::Installer::Download(std::string url, std::string dir, bool Progress)
{
    try
    {
        // Class for write data on windows
        WriteData writer;
        withProgress = Progress;
        // Get name of file from url
        std::string name = (url.substr(url.find_last_of("/")));
        std::string filename = dir + "/" + name.replace(name.find("/"), 1, "");
        // Call method for download file
        HRESULT Download = URLDownloadToFile(NULL, url.c_str(), filename.c_str(), 0, static_cast<IBindStatusCallback *>(&writer));
        switch (Download)
        {
        case -2146697211:
            throw std::domain_error(translate["NoInternetConnection"].asCString());
        case -2147467260:
            throw std::domain_error(translate["ConnectionReset"].asCString());
        }
        if (Progress == true)
        {
            // If the progress bar is not completely filled in, then paint over manually
            if (Bar::Process < 100 && Bar::Process != Percentage)
            {
                for (int i = (Bar::Process - 1); i < 98; i++)
                {
                    progressbar.Update(LastSize, LastTotalSize);
                }
                progressbar.Update(LastTotalSize, LastTotalSize);
            }
            // Reset all variables and preferences
            progressbar.ResetAll();
            Percentage = 0;
            TempPercentage = 0;
        }
    }
    catch (std::exception &error)
    {
        std::string ErrorText = "==> ❌ " + std::string(error.what());
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "Download()", error.what());
        std::cerr << ErrorText << std::endl;
    }
}

void Windows::Installer::AddToPATH()
{
    std::string Command = "C:\\ProgramData\\DeepForge\\DeepForge-Toolset\\Temp\\pathman-v0.5.2-windows-amd64.exe add " + ApplicationDir + " && del C:\\ProgramData\\DeepForge\\DeepForge-Toolset\\Temp\\pathman-v0.5.2-windows-amd64.exe";
#if defined(_M_AMD64)
    Download(PathmanURL_AMD64, TempFolder, false);
#elif _M_ARM64
    Download(PathmanURL_ARM64, NewTempFolder, false);
#endif
    system(Command.c_str());
}

// Method for create symlink on desktop
void Windows::Installer::CreateSymlink(std::string nameSymlink, std::string filePath)
{
    nameSymlink = nameSymlink + ".exe";
    filePath = filePath + ".exe";
    char *UserFolder = getenv("USERPROFILE");
    std::string symlinkPath = std::string(UserFolder) + "\\Desktop\\" + nameSymlink;
    CreateHardLinkA(symlinkPath.c_str(), filePath.c_str(), NULL);
}

/* The 'MakeDirectory' function is used to create a directory (folder) in the file system.*/
void Windows::Installer::MakeDirectory(std::string dir)
{
    try
    {
        std::string currentDir;
        std::string fullPath = "";
        std::string delimiter = "\\";
        size_t pos = 0;
        while ((pos = dir.find(delimiter)) != std::string::npos)
        {
            currentDir = dir.substr(0, pos);
            if (fullPath != "")
            {
                fullPath = fullPath + "\\" + currentDir;
                if (std::filesystem::exists(fullPath) == false)
                {
                    std::filesystem::create_directory(fullPath);
                }
            }
            else
            {
                fullPath = currentDir + "\\";
            }
            dir.erase(0, pos + delimiter.length());
        }
        if (fullPath != "")
        {
            fullPath = fullPath + "\\" + dir;
        }
        else
        {
            fullPath = dir + "\\";
        }
        if (std::filesystem::exists(fullPath) == false)
        {
            std::filesystem::create_directory(fullPath);
        }
    }
    catch (std::exception &error)
    {
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "MakeDirectory()", error.what());
        std::cerr << error.what() << std::endl;
    }
}



void Windows::Installer::AddToStartupSystem(std::string filePath)
{
    filePath = filePath + ".exe";
    HKEY hKey;
    const char *czStartName = "DeepForge-UpdateManager";
    // const char* czExePath   = "C:\\Users\\user\\AppData\\Roaming\\Microsoft\\Windows\\DeepForgeToolset.exe";

    LONG lnRes = RegOpenKeyEx(HKEY_CURRENT_USER,
                              "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                              0, KEY_WRITE,
                              &hKey);
    if (ERROR_SUCCESS == lnRes)
    {
        lnRes = RegSetValueEx(hKey,
                              czStartName,
                              0,
                              REG_SZ,
                              (unsigned char *)filePath.c_str(),
                              strlen(filePath.c_str()));
    }

    RegCloseKey(hKey);
}

void Windows::Installer::RebootSystem()
{
    system("shutdown -r -t 0");
}