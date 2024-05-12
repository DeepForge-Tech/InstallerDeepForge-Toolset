import platform
import os

delimiter = "========================================================"


class Linux:

    def __init__(self):
        self.YUM_PACKAGES = (
            "boost-devel xterm wget make cmake gcc-c++ curl libcurl sqlite-devel openssl-devel gtest-devel gtest gmock gmock-devel"
        )
        self.DEB_PACKAGES = "libboost-all-dev libasio-dev xterm wget openssl libssl-dev g++ gcc build-essential cmake make curl libcurl4-openssl-dev libjsoncpp-dev libfmt-dev libsqlite3-dev libgtest-dev googletest google-mock libgmock-dev libtbb-dev libzip-dev zlib1g-dev"
        self.PACMAN_PACKAGES = "boost asio xterm wget jsoncpp gcc base-devel cmake gtest libcurl-compat libcurl-gnutls curl fmt sqlite sqlite-tcl zlib openssl libzip"
        # self.ZYPPER_PACKAGES = "xterm wget libcurl-devel gcc-c++ cmake gtest gmock zlib-devel fmt-devel sqlite3-devel jsoncpp-devel"
        self.distribution = platform.freedesktop_os_release()["NAME"]
        self.architecture = platform.architecture()[0]
        self.PACKAGES = {
            "CentOS Linux": self.YUM_PACKAGES,
            "Red Hat Enterprise Linux Server": self.YUM_PACKAGES,
            "Fedora Linux": self.YUM_PACKAGES,
            "Ubuntu": self.DEB_PACKAGES,
            "Debian GNU/Linux": self.DEB_PACKAGES,
            "Linux Mint": self.DEB_PACKAGES,
            "Knoppix": self.DEB_PACKAGES,
            "Raspbian GNU/Linux": self.DEB_PACKAGES,
            "Manjaro Linux": self.PACMAN_PACKAGES,
            "Manjaro ARM": self.PACMAN_PACKAGES,
            "Manjaro AMD64": self.PACMAN_PACKAGES,
            "Arch Linux": self.PACMAN_PACKAGES,
            "Kali GNU/Linux": self.DEB_PACKAGES,
            # "openSUSE Leap": self.ZYPPER_PACKAGES,
            # "openSUSE Tumbleweed": self.ZYPPER_PACKAGES
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
            "Kali GNU/Linux": "apt",
            "Manjaro ARM": "pacman",
            "Manjaro AMD64": "pacman",
            # "openSUSE Leap": "zypper",
            # "openSUSE Tumbleweed": "zypper"
        }

    def start(self) -> int:
        if self.distribution == "CentOS Linux":
            command = "cd /etc/yum.repos.d/ && sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-* && sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*"
            os.system(command)
        if self.INSTALLERS[self.distribution] == "yum" or self.INSTALLERS[self.distribution] == "apt":
            command = self.INSTALLERS[
                self.distribution] + " install " + "sudo -y"
            os.system(command)
            command = self.INSTALLERS[self.distribution] + " update " + "-y"
            os.system(command)
        elif self.INSTALLERS[self.distribution] == "pacman":
            # command = self.INSTALLERS[self.distribution] + " -Sy " + "sudo"
            # os.system(command)
            command = "sudo " + self.INSTALLERS[self.distribution] + " -Suy --noconfirm"
            os.system(command)
        success_installed = 0
        failed_packages = []
        packages = self.PACKAGES[self.distribution].split()
        for package in packages:
            if self.INSTALLERS[self.distribution] == "yum" or self.INSTALLERS[self.distribution] == "apt":
                command = ("sudo -s " + self.INSTALLERS[self.distribution] +
                           " install " + package + " -y")
                install_result = os.system(command)
            elif self.INSTALLERS[self.distribution] == "pacman":
                command = ("sudo -s " + self.INSTALLERS[self.distribution] +
                           " -Sy " + package + " --noconfirm")
                install_result = os.system(command)
            if install_result == 0:
                success_installed += 1
            else:
                failed_packages.append(package)
        if self.INSTALLERS[self.distribution] == "apt":
            command = "sudo apt autoremove -y"
            os.system(command)
        elif self.INSTALLERS[self.distribution] == "pacman":
            command = "sudo pacman -Scc --noconfirm"
            os.system(command)
        print(delimiter)
        print(
            f"==> Successfully installed: {success_installed} package(s)\nFailed to install: {len(packages) - success_installed} package(s)"
        )
        if len(failed_packages) > 0:
            print("==> Reinstall the packages:")
            i = 1
            for package in failed_packages:
                print(f"{i}.{package}")
                i += 1
        return 502


class Windows:

    def __init__(self):
        self.architecture = platform.architecture()[0]

    def checkMSBuild(self) -> bool:
        distributions = ["Enterprise", "Community", "Proffesional"]
        vs_path = "C:\\Program Files\\Microsoft Visual Studio\\"
        msbuild_path = "C:\\Program Files (x86)\\Microsoft Visual Studio\\"
        if os.path.exists(vs_path):
            for obj in os.listdir(vs_path):
                if os.path.isdir(os.path.join(vs_path,
                                              obj)) and obj.startswith("20"):
                    for distribution in distributions:
                        if distribution in os.listdir(
                                os.path.join(vs_path, obj)):
                            return len(
                                os.listdir(
                                    os.path.join(vs_path, obj,
                                                 distribution))) > 0
        elif os.path.exists(msbuild_path):
            for obj in os.listdir(msbuild_path):
                if os.path.isdir(os.path.join(msbuild_path,
                                              obj)) and obj.startswith("20"):
                    for distribution in distributions:
                        if distribution in os.listdir(
                                os.path.join(msbuild_path, obj)):
                            return len(
                                os.listdir(
                                    os.path.join(msbuild_path, obj,
                                                 distribution))) > 0

    def installVCpkg(self) -> int:
        check_command = "vcpkg --help"
        result = os.system(check_command)
        if result != 0:
            os.chdir("C:\\")
            install_ommand = "git clone https://github.com/microsoft/vcpkg"
            result = os.system(install_ommand)
            if result == 0:
                install_ommand = ".\\vcpkg\\bootstrap-vcpkg.bat -disableMetrics"
                result_install = os.system(install_ommand)
                return result_install
        return result

    def installCMake(self) -> int:
        check_command = "cmake --help"
        result = os.system(check_command)
        if result != 0:
            install_command = "winget install -e --id Kitware.CMake"
            result = os.system(install_command)
        return result

    def installCrow(self) -> int:
        Command = "vcpkg install crow"
        result = os.system(Command)
        return result

    def printError(self, result, nameProgram):
        if result != 0:
            print(delimiter)
            print(f"==> Failed to install {nameProgram}")
            return 502

    def start(self) -> int:
        try:
            result = self.checkMSBuild()
            if not result:
                print(
                    "For further development, Visual Studio or MSBuild is required.\nVisual Studio download link: https://visualstudio.microsoft.com/ru/vs/\nMSBuild download link: https://aka.ms/vs/17/release/vs_BuildTools.exe"
                )

            command = (
                "powershell -noprofile -executionpolicy bypass -File .\InstallWinGet.ps1"
            )
            resultInstallWinGet = os.system(command)
            self.printError(result, "WinGet")

            result = self.installCMake()
            self.printError(result, "CMake")

            result = self.installVCpkg()
            self.printError(result, "VCpkg")

            result = self.installCrow()
            self.printError(result, "Crow")
            return resultInstallWinGet
        except Exception as error:
            print(error)


class macOS:

    def __init__(self):
        self.architecture = platform.architecture()[0]

    def checkXcodeCommandLineToolset(self) -> bool:
        path = "/Library/Developer/CommandLineTools//usr/bin/"
        return os.path.exists(path) and len(os.listdir(path)) > 0

    def start(self) -> int:
        command = '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
        result = os.system(command)
        if result != 0:
            print(delimiter)
            print("==> Failed to install Homebrew")
            return 502
        command = "brew install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc zlib cmake libzip openssl wget boost asio"
        result = os.system(command)
        check_build_tools = self.checkXcodeCommandLineToolset()
        if check_build_tools == True:
            return result
        else:
           return 502

if __name__ == "__main__":
    platforms = {"Linux": Linux, "Windows": Windows, "Darwin": macOS}
    checker = platforms[platform.system()]()
    print("==> Installing packages...")
    result = checker.start()
    if result != 502:
        print(delimiter)
        print("==> All packages installed successfully")
