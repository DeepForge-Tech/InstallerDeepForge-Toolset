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
#include <InstallerDeepForge-Toolset/Linux.hpp>

/*  The `unpackArchive` function takes two parameters: `path_from` and `path_to`.
    It uses the `Unzipper` class to extract the contents of an archive file located at `path_from` and saves them to the directory specified by `path_to`.
    After extracting the contents, the function closes the `Unzipper` object.
*/
void Linux::Installer::UnpackArchive(std::string path_from, std::string path_to)
{
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
            if (endsWith(output_path, "/"))
            {
                MakeDirectory(output_path);
            }
            else
            {
                std::ofstream out(output_path, std::ios::binary);
                if (!out)
                {
                    std::cerr << "Failed to create file: " << output_path << std::endl;
                    continue;
                }
                size_t fileSize = file_stat.m_uncomp_size;
                void *fileData = mz_zip_reader_extract_to_heap(&zip_archive, file_stat.m_file_index, &fileSize, 0);
                if (!fileData)
                {
                    throw std::runtime_error("Failed to extract file: " + std::string(file_stat.m_filename));
                }

                out.write(static_cast<const char *>(fileData), fileSize);
                mz_free(fileData);

                out.close();
            }
        }

        mz_zip_reader_end(&zip_archive);
    }
    catch (std::exception &error)
    {
        std::string logText = "==> ❌ Function: UnpackArchive." + std::string(error.what());
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "UnpackArchive()", error.what());
        std::cerr << logText << std::endl;
    }
}

void Linux::Installer::download(std::string url, std::string dir, bool Progress)
{
    try
    {
        withProgress = Progress;
        std::string name = (url.substr(url.find_last_of("/")));
        std::string filename = dir + "/" + name.replace(name.find("/"), 1, "");
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
        curl_easy_cleanup(curl);
        fclose(file);
        /* The bellow code is checking the value of the variable "response" and performing
        different actions based on its value. If the value of "response" is not equal to
        CURLE_OK, it enters a switch statement. Inside the switch statement, it checks the
        value of "response" against different cases and performs specific actions for each
        case. The actions involve writing and logging different error messages based on the
        value of "response". */
        if (response != CURLE_OK)
        {
            switch (response)
            {
            case CURLE_COULDNT_CONNECT:
                throw std::domain_error(translate["LOG_ERROR_CURLE_COULDNT_CONNECT"].asString());
                break;
            case CURLE_COULDNT_RESOLVE_HOST:
                throw std::domain_error(translate["LOG_ERROR_CURLE_COULDNT_RESOLVE_HOST"].asString());
                break;
            case CURLE_COULDNT_RESOLVE_PROXY:
                throw std::domain_error(translate["LOG_ERROR_CURLE_COULDNT_RESOLVE_PROXY"].asString());
                break;
            case CURLE_UNSUPPORTED_PROTOCOL:
                throw std::domain_error(translate["LOG_ERROR_CURLE_UNSUPPORTED_PROTOCOL"].asString());
                break;
            case CURLE_SSL_CONNECT_ERROR:
                throw std::domain_error(translate["LOG_ERROR_CURLE_SSL_CONNECT_ERROR"].asString());
                break;
            }
        }
        if (Progress == true)
        {
            // If the progress bar is not completely filled in, then paint over manually
            for (int i = progressbar.progress; i < 100; i++)
            {
                progressbar.update(LastSize, LastTotalSize);
            }
            // Reset all variables and preferences
            progressbar.resetAll();
            Percentage = 0;
            TempPercentage = 0;
        }
    }
    catch (std::exception &error)
    {
        std::string logText = "==> ❌ " + std::string(error.what());
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "download()", error.what());
        std::cerr << logText << std::endl;
    }
}

/*The `makeDirectory` function is responsible for creating a directory (folder) in the file system.*/
void Linux::Installer::makeDirectory(std::string dir)
{
    try
    {
        std::string currentDir;
        std::string fullPath = "";
        std::string delimiter = "/";
        size_t pos = 0;
        while ((pos = dir.find(delimiter)) != std::string::npos)
        {
            currentDir = dir.substr(0, pos);
            if (fullPath != "")
            {
                fullPath = fullPath + "/" + currentDir;
                if (std::filesystem::exists(fullPath) == false)
                {
                    std::filesystem::create_directory(fullPath);
                }
            }
            else
            {
                fullPath = "/" + currentDir;
            }
            dir.erase(0, pos + delimiter.length());
        }
        fullPath = fullPath + "/" + dir;
        if (std::filesystem::exists(fullPath) == false)
        {
            std::filesystem::create_directory(fullPath);
        }
    }
    catch (std::exception &error)
    {
        logger.sendError(NameProgram, Architecture, __channel__, OS_NAME, "makeDirectory()", error.what());
    }
}
void Linux::Installer::createSymlink(std::string nameSymlink, std::string filePath)
{
    // char *UserFolder = getenv("HOME");
    std::string symlinkPath = std::string(UserFolder) + "/Desktop/" + nameSymlink;
    std::string command = "sudo ln -s " + filePath + " " + symlinkPath;
    system(command.c_str());
}

void Linux::Installer::addToStartupSystem()
{
    std::string ServicePath = TempFolder + "/DeepForge-UpdateManager.service";
    std::string command = "sudo mv " + ServicePath + " /etc/systemd/system/DeepForge-UpdateManager.service && sudo chmod 644 /etc/systemd/system/DeepForge-UpdateManager.service && sudo systemctl enable /etc/systemd/system/DeepForge-UpdateManager.service";
    download(SERVICE_URL, TempFolder, false);
    system(command.c_str());
}

void Linux::Installer::installLibraries()
{
    std::string ShellScriptPath;
    std::string command;
    download(SHELL_SCRIPT_URL, TempFolder, false);
    // name = (ShellScript_URL.substr(ShellScript_URL.find_last_of("/")));
    ShellScriptPath = TempFolder + "/" + "InstallPackages.sh";
    command = "python3 " + ShellScriptPath;
    system(command.c_str());
}

void Linux::Installer::rebootSystem()
{
    system("sudo shutdown -r now");
}

vvoid Linux::Installer::addPath()
{
    system("export PATH=\"$PATH:" + ApplicationFolder + '"');
}