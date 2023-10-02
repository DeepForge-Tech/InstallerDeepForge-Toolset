#!/bin/bash
echo "==> Installing libraries"
# Installing libraries
YUM_PACKAGE_NAME="jsoncpp jsoncpp-devel make cmake g++ gcc gtest-devel gtest gmock-devel gmock curl libcurl-devel libcurl sqlite-devel sqlite-tcl libstdc++.x86_64 libstdc++-devel.x86_64 libstdc++-static.x86_64 zlib"
DEB_PACKAGE_NAME="g++ gcc build-essential cmake make curl libcurl4-openssl-dev libjsoncpp-dev libfmt-dev libsqlite3-dev libgtest-dev googletest google-mock libgmock-dev libtbb-dev libzip-dev zlib1g-dev"
PACMAN_PACKAGE_NAME="jsoncpp gcc base-devel cmake  clang gtest lib32-curl libcurl-compat libcurl-gnutls curl fmt lib32-sqlite sqlite sqlite-tcl zlib"
ZYPPER_PACKAGE_NAME=""
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
   if [ -f /etc/os-release ]; then
      # freedesktop.org and systemd
      . /etc/os-release
      OS=$NAME
   elif type lsb_release >/dev/null 2>&1; then
      # linuxbase.org
      OS=$(lsb_release -si)
   elif [ -f /etc/lsb-release ]; then
      # For some versions of Debian/Ubuntu without lsb_release command
      . /etc/lsb-release
      OS=$DISTRIB_ID
   elif [ -f /etc/debian_version ]; then
      # Older Debian/Ubuntu/etc.
      OS=Debian
   elif [ -f /etc/SuSe-release ]; then
      # Older SuSE/etc.
      ...
   elif [ -f /etc/redhat-release ]; then
      # Older Red Hat, CentOS, etc.
      ...
   else
      # Fall back to uname, e.g. "Linux <version>", also works for BSD, etc.
      OS=$(uname -s)
   fi
   if [[ "$OS" == "CentOS"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo yum update -y
      sudo yum install -y $YUM_PACKAGE_NAME
   elif [[ "$OS" == "Red"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo yum update -y
      sudo yum install -y $YUM_PACKAGE_NAME
   elif [[ "$OS" == "Fedora"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo yum update -y
      sudo yum install -y $YUM_PACKAGE_NAME
   elif [[ "$OS" == "Ubuntu"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Debian"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Mint"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Knoppix"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Manjaro Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo pacman -Sy jsoncpp
      sudo pacman -Sy gcc
      sudo pacman -Sy base-devel
      sudo pacman -Sy cmake
      sudo pacman -Sy clang
      sudo pacman -Sy gtest
      sudo pacman -Sy lib32-curl
      sudo pacman -Sy libcurl-compat
      sudo pacman -Sy libcurl-gnutls
      sudo pacman -Sy curl
      sudo pacman -Sy fmt
      sudo pacman -Sy lib32-sqlite
      sudo pacman -Sy sqlite
      sudo pacman -Sy sqlite-tcl
      sudo pacman -Sy zlib
   elif [[ "$OS" == "Kali GNU/Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   else
      echo "Not found package manager"
      exit 1;
   fi
   echo "==> Building library Zipper"
   cd src
   mkdir lib
   cd ..
   git clone --recursive https://github.com/sebastiandev/zipper.git
   cd zipper
   mkdir build
   cd build
   cmake ../
   make
   find . -name "*.a" -exec mv "{}" ../../src/lib/ \;
   # find . -name "*.so" -exec mv "{}" ../../src/lib \;
   cd .. && cd ..
   sudo rm -rf ./zipper
   echo "==> Zipper successfully builded"
elif [[ "$OSTYPE" == "darwin"* ]]; then
   # Mac OSX
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   brew  install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc zlib cmake
   echo "==> Building library Zipper"
   git clone --recursive https://github.com/sebastiandev/zipper.git
   cd zipper
   mkdir build
   cd build
   cmake ../
   make
   find . -name "*.a" -exec mv "{}" ../../src/lib/ \;
   cd .. && cd ..
   sudo rm -rf ./zipper
   echo "==> Build of Zipper finished"
fi
echo "==> Libraries successfully installed"
unameOut=$(uname -a)
case "${unameOut}" in
	Darwin*) 	os="macOS";;
	Linux*)		os="Linux";;
esac
# Building
echo "==> Building"
sudo rm -rf ./build/$os
mkdir build
cd build
mkdir $os
cd ..
echo "==> Building project"
case "${unameOut}" in
	Darwin*) 	sudo clang++ -o ./build/$os/InstallerDeepForge-Toolset ./src/InstallerDeepForge_Toolset.cpp -DCURL_STATICLIB -I ../../include -I ./src/include -L ./src/lib -lZipper -lz -lcurl -lsqlite3 -std=c++2a -Bstatic;;
	Linux*)		sudo g++ -o ./build/$os/InstallerDeepForge-Toolset ./src/InstallerDeepForge_Toolset.cpp -DCURL_STATICLIB -I ../../include -I ./src/include -L ./src/lib -lZipper -lz -lcurl -lsqlite3 -std=c++2a -Bstatic;;
esac
echo "==> Build of project finished"
cd build
cd $os
case "${unameOut}" in
	Darwin*) 	./InstallerDeepForge-Toolset;;
	Linux*)		sudo ./InstallerDeepForge-Toolset;;
esac

#echo "=================================="
exit 0