from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
from conan.tools.layout import cmake_layout

class SynodicCoreConan(ConanFile):
	name = "synodic-template-library"
	version = "0.1.0"
	license = "MIT"
	homepage = "https://github.com/Synodic-Software/Synodic-Template-Library"
	settings = "os", "compiler", "build_type", "arch"
	requires = "catch2/2.13.7"
	generators = "CMakeToolchain", "CMakeDeps"

	def layout(self):
		cmake_layout(self)

	def generate(self):
		toolchain = CMakeToolchain(self, generator='Ninja')
		toolchain.variables["BUILD_TESTING"] = True
		toolchain.generate()

	def build(self):
		cmake = CMake()
		cmake.configure()
		cmake.build()

	def package(self):
		cmake = CMake()
		cmake.install()