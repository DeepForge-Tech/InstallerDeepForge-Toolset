#!/bin/bash
echo "==> Installing libraries"
# Installing libraries
if [[ "$OSTYPE" == "darwin"* ]]; then
   # Mac OSX
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   brew install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc zlib cmake libzip openssl wget
fi
echo "==> Libraries successfully installed"
exit 0
