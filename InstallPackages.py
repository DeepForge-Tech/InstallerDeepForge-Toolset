import platform
import os

delimiter = "========================================================"

class Linux:
    def __init__(self):
        self.YUM_PACKAGES = "xterm wget make cmake gcc-c++ curl libcurl sqlite-devel openssl-devel"
        self.DEB_PACKAGES = "xterm wget openssl libssl-dev g++ gcc build-essential cmake make curl libcurl4-openssl-dev libjsoncpp-dev libfmt-dev libsqlite3-dev libgtest-dev googletest google-mock libgmock-dev libtbb-dev libzip-dev zlib1g-dev"
        self.PACMAN_PACKAGES = "xterm wget jsoncpp gcc base-devel cmake gtest lib32-curl libcurl-compat libcurl-gnutls curl fmt lib32-sqlite sqlite sqlite-tcl zlib openssl lib32-openssl openssl-1.1 libzip"
        self.ZYPPER_PACKAGES = "xterm wget libcurl-devel gcc-c++ cmake gtest gmock zlib-devel fmt-devel sqlite3-devel jsoncpp-devel"
        self.distribution = platform.freedesktop_os_release()["NAME"]
        self.architecture = platform.architecture()[0]
        self.PACKAGES = {
            "CentOS Linux":self.YUM_PACKAGES,
            "Red Hat Enterprise Linux Server":self.YUM_PACKAGES,
            "Fedora Linux":self.YUM_PACKAGES,
            "Ubuntu":self.DEB_PACKAGES,
            "Debian GNU/Linux":self.DEB_PACKAGES,
            "Linux Mint":self.DEB_PACKAGES,
            "Knoppix":self.DEB_PACKAGES,
            "Raspbian GNU/Linux":self.DEB_PACKAGES,
            "Manjaro Linux":self.PACMAN_PACKAGES,
            "Arch Linux":self.PACMAN_PACKAGES,
            "Kali GNU/Linux":self.DEB_PACKAGES
        }
        self.installer = ""
        self.INSTALLERS = {
            "CentOS Linux": "yum",
            "Red Hat Enterprise Linux Server": "yum",
            "Fedora Linux": "yum",
            "Ubuntu": "apt",
            "Debian GNU/Linux": "apt",
            "Linux Mint": "apt",
            "Knoppix": "apt",
            "Raspbian GNU/Linux": "apt",
            "Manjaro Linux": "pacman",
            "Arch Linux": "pacman",
            "Kali GNU/Linux": "apt"
        }

    def start(self) -> int:
        command = self.INSTALLERS[self.distribution] + " install " + "sudo -y"
        result = os.system(command)
        if result == 0:
            success_installed = 0
            failed_packages = []
            print(self.PACKAGES[self.distribution].split())
            packages = self.PACKAGES[self.distribution].split()
            for package in packages:
                command = "sudo -s " + self.INSTALLERS[self.distribution] + " install " + package + " -y"
                install_result = os.system(command)
                if install_result == 0:
                    success_installed += 1
                else:
                    failed_packages.append(package)
            print(delimiter)
            print(f"Successfully installed: {success_installed} package(s)\nFailed to install: {len(packages) - success_installed} package(s)")    
            print("Reinstall the packages:")
            i = 1
            for package in failed_packages:
                print(f"{i}.{package}")
                i += 1
        return 502
                                
class Windows:
    def __init__(self):
        self.architecture = platform.architecture()[0]

    def start(self) -> int:
        command = "powershell -noprofile -executionpolicy bypass -File .\InstallWinGet.ps1"
        result = os.system(command)
        if result != 0:
            print(delimiter)
            print("Failed to install WinGet")
            return 502
        return result

class macOS:
    def __init__(self):
        self.architecture = platform.architecture()[0]

    def start(self) -> int:
        command = "/bin/bash -c '$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)'"
        result = os.system(command)
        if result != 0:
            print(delimiter)
            print("Failed to install Homebrew")
            return 502
        command = "brew install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc zlib cmake libzip openssl wget"
        result = os.system(command)
        return result

platforms = {"Linux": Linux, "Windows": Windows, "Darwin": macOS}

if __name__ == "__main__":
    checker = platforms[platform.system()]()
    result = checker.start()
    if result != 502:
        print(delimiter)
        print("All packages installed successfully")