import platform

class Linux:
    def __init__(self):
        self.YUM_PACKAGES = []
        self.DEB_PACKAGES = []
        self.PACMAN_PACKAGES = []
        self.ZYPPER_PACKAGES = []
        self.distribution = platform.freedesktop_os_release()["NAME"]
        self.architecture = platform.architecture()[0]
        print(self.architecture)

    # def 
    # def start():


class Windows:
    def __init__(self):
        pass

class macOS:
    def __init__(self):
        pass

platforms = {"Linux": Linux, "Windows": Windows, "Darwin": macOS}

if __name__ == "__main__":
    checker = platforms[platform.system()]()