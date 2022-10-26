from conans import ConanFile, CMake

prefixChannel = "@YOUR-PREFIX/YOUR-CHANNEL"


class SandboxConan(ConanFile):
    settings = "os", "arch", "compiler"
    generators = "cmake"
    requires = [
        f"glad/0.1.36{prefixChannel}",
        f"glfw/3.3.5{prefixChannel}",
        f"imgui/1.86.0{prefixChannel}"
    ]
