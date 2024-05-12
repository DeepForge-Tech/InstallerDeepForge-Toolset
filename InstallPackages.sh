#!/bin/bash
# Installing libraries
if [[ "$OSTYPE" == "darwin"* ]]; then
   # Mac OSX
   echo "==> Installing libraries"
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   brew install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc cmake libzip openssl wget
   echo "==> Libraries successfully installed"
fi
exit 0
