@echo off
set arg_1 = %1
echo -- Building project
if exist .\build\Windows (
    if ERRORLEVEL 0 (
        g++ -o .\build\Windows\InstallerDeepForge-Toolset.exe .\src\InstallerDeepForge_Toolset.cpp -I ..\..\include  -L ..\..\lib\ -I .\src\include -L .\src\lib -static -static-libgcc -static-libstdc++ -lurlmon -lcurl -lsqlite3 -lws2_32 -lwinmm -std=c++20  .\src\resource.res -lZipper -lz -w
        echo -- Build of project finished
    ) else (
        echo -- Error in project build.
    )
) else (
    mkdir build
    cd build
    mkdir Windows
    cd ..
    if ERRORLEVEL 0 (
        g++ -o .\build\Windows\InstallerDeepForge-Toolset.exe .\src\InstallerDeepForge_Toolset.cpp -I ..\..\include  -L ..\..\lib\ -I .\src\include -L .\src\lib -static -static-libgcc -static-libstdc++ -lurlmon -lcurl -lsqlite3 -lws2_32 -lwinmm -std=c++20  .\src\resource.res -lZipper -lz -w
        echo -- Build of project finished
    ) else (
        echo -- Error in project build.
    )
)

echo ==================================

if arg_1=="-autostart" or arg_1=="-AutoStart" or arg_1=="-Autostart" (
   echo -- Running ".\build\Windows\InstallerDeeepForgeToolset.exe"
   cd build
   cd Windows
   .\InstallerDeepForge-Toolset.exe
)