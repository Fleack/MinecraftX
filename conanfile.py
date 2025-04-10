from conan import ConanFile
from conan.tools.cmake import cmake_layout


class MinecraftConan(ConanFile):
    name = "minecraft"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = [
        "glfw/3.4",
        "glm/0.9.9.8",
        "stb/cci.20210713",
        "imgui/1.89.9",
        "spdlog/1.15.1",
        "catch2/3.8.0",
        "fastnoise-lite/1.1.1@local/testing",
        "glad/2.0.8@local/testing",
        "concurrencpp/0.1.7"
    ]
    default_options = {
        "glfw/*:shared": False,
        "spdlog/*:header_only": False,
        "glad/*:shared": False,
        "imgui/*:shared": False,
        "catch2/*:with_main": False
    }

    def layout(self):
        cmake_layout(self)
