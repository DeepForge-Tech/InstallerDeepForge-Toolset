import platform
import os

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

    def start():
        command = self.INSTALLERS[self.distribution] + " install" + "sudo"
        os.system(command)
        for package in self.PACKAGES[self.distribution].split():
            os.system("sudo -s " + self.INSTALLERS[self.distribution] + " install" + package)
                                
        
        

class Windows:
    def __init__(self):
        pass

class macOS:
    def __init__(self):
        pass

platforms = {"Linux": Linux, "Windows": Windows, "Darwin": macOS}

if __name__ == "__main__":
    checker = platforms[platform.system()]()
    checker.start()