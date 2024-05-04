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
#ifndef MACOS_HPP_
#define MACOS_HPP_
#include <InstallerDeepForge-Toolset/Advanced.hpp>

namespace macOS
{
    class Installer
    {
        public:
        protected:
        void Download(std::string url, std::string dir, bool Progress);
        /* The `CreateSymlink` function is creating a symbolic link (symlink) in the `/Applications` directory. It takes two parameters: `nameSymlink` which is the name of the symlink, and `filePath` which is the path to the file or directory that the symlink will point to.*/
        void CreateSymlink(std::string nameSymlink, std::string filePath);
        /*The `MakeDirectory` function is responsible for creating a directory (folder) in the file system.*/
        void MakeDirectory(std::string dir);
        void UnpackArchive(std::string path_from, std::string path_to);
        void AddToStartupSystem();
        /* The `InstallLibraries()` function is responsible for downloading and executing a shell script that installs additional libraries or dependencies required by the DeepForge Toolset. */
        void InstallLibraries();
        /* The `RebootSystem()` function is responsible for rebooting the system. It uses the `system()` function to execute the command `sudo shutdown -r now`, which instructs the system to restart immediately. */
        void RebootSystem();
        void AddToPATH();
    };
}

#endif