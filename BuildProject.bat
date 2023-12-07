@echo off
set arg_1 = %1
echo -- Building library Zipper
git clone --recursive https://github.com/sebastiandev/zipper.git
cd src
mkdir lib
cd ..
cd zipper
mkdir build
cmake  .
cmake --build .
rename "libstaticZipper.a" "libZipper.a"
Xcopy *.a  ..\src\lib\  /Y
cd ..
@REM rmdir .\zipper /s /q
echo - Zipper successfully builded
echo ==================================
echo -- Building project
if exist .\build\Windows (
    if ERRORLEVEL 0 (
        g++ -o .\build\Windows\InstallerDeepForge-Toolset.exe .\src\InstallerDeepForge_Toolset.cpp -DCURL_STATICLIB -I ..\..\include  -L ..\..\lib\ -I .\src\include -L .\src\lib -static -static-libgcc -static-libstdc++ -lws2_32 -lwinmm -lurlmon -lsqlite3 -ljsoncpp -std=c++20  .\src\resource.res -lZipper -lz -w -Bstatic
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
        g++ -o .\build\Windows\InstallerDeepForge-Toolset.exe .\src\InstallerDeepForge_Toolset.cpp -DCURL_STATICLIB -I ..\..\include  -L ..\..\lib\ -I .\src\include -L .\src\lib -static -static-libgcc -static-libstdc++ -lws2_32 -lwinmm -lurlmon -lsqlite3 -ljsoncpp -std=c++20  .\src\resource.res -lZipper -lz -w -Bstatic
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