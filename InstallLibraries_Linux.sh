#!/bin/bash
echo "==> Installing libraries"
# Installing libraries
YUM_PACKAGE_NAME="xterm wget make cmake gcc-c++ curl libcurl sqlite-devel openssl-devel"
DEB_PACKAGE_NAME="xterm wget openssl libssl-dev g++ gcc build-essential cmake make curl libcurl4-openssl-dev libjsoncpp-dev libfmt-dev libsqlite3-dev libgtest-dev googletest google-mock libgmock-dev libtbb-dev libzip-dev zlib1g-dev"
PACMAN_PACKAGE_NAME="xterm wget jsoncpp gcc base-devel cmake gtest lib32-curl libcurl-compat libcurl-gnutls curl fmt lib32-sqlite sqlite sqlite-tcl zlib openssl lib32-openssl openssl-1.1 libzip"
ZYPPER_PACKAGE_NAME="xterm wget libcurl-devel gcc-c++ cmake gtest gmock zlib-devel fmt-devel sqlite3-devel jsoncpp-devel"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
   DISTRO=$(grep -E '^NAME=' /etc/os-release | cut -d '"' -f2)
   if [[ "$DISTRO" == "CentOS Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo yum update -y
      sudo yum install -y $YUM_PACKAGE_NAME
   elif [["$DISTRO"=="Red Hat Enterprise Linux Server"*]];then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo yum update -y
      sudo yum install -y $YUM_PACKAGE_NAME
   elif [[ "$DISTRO" == "Fedora Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo yum update -y
      sudo yum install -y $YUM_PACKAGE_NAME
   elif [[ "$DISTRO" == "Ubuntu"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$DISTRO" == "Debian GNU/Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Linux Mint"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$DISTRO" == "Knoppix"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$DISTRO" == "Raspbian GNU/Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$DISTRO" == "Manjaro Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo pacman -Syu --noconfirm
      sudo pacman -Sy $PACMAN_PACKAGE_NAME --noconfirm
   elif [[ "$DISTRO" == "Arch Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo pacman -Syu --noconfirm
      sudo pacman -Sy $PACMAN_PACKAGE_NAME --noconfirm
   elif [[ "$DISTRO" == "Kali GNU/Linux"* ]]; then
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
fi
export PATH=$PATH:/snap/bin
echo "==> Libraries successfully installed"
exit 0
