from conans import ConanFile, CMake

requirements = [
    "glad/0.1.36",
    "GLFW/3.3.5",
    "DearImGui/1.86.0"
]


class SandboxConan(ConanFile):
    settings = "os", "arch", "compiler"
    generators = "cmake"

    def configure(self):

        for r in requirements:
            self.requires(
                f"{r}@YOUR-PREFIX/public"
            )
