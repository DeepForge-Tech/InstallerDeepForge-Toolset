#!/bin/bash
echo "==> Installing libraries"
# Installing libraries
YUM_PACKAGE_NAME="make cmake gcc-c++ curl libcurl sqlite-devel openssl-devel"
DEB_PACKAGE_NAME="g++ gcc build-essential cmake make curl libcurl4-openssl-dev libjsoncpp-dev libfmt-dev libsqlite3-dev libgtest-dev googletest google-mock libgmock-dev libtbb-dev libzip-dev zlib1g-dev"
PACMAN_PACKAGE_NAME="jsoncpp gcc base-devel cmake  clang gtest lib32-curl libcurl-compat libcurl-gnutls curl fmt lib32-sqlite sqlite sqlite-tcl zlib"
ZYPPER_PACKAGE_NAME="libcurl-devel gcc-c++ cmake gtest gmock zlib-devel fmt-devel sqlite3-devel jsoncpp-devel"
if [[ "$OSTYPE" == "darwin"* ]]; then
   # Mac OSX
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   brew install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc zlib cmake
fi
echo "==> Libraries successfully installed"
exit 0