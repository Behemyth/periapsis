from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain
from conan.tools.layout import cmake_layout

class SynodicCoreConan(ConanFile):
	name = "synodic-template-library"
	version = "0.1.0"
	license = "MIT"
	homepage = "https://github.com/Synodic-Software/Synodic-Template-Library"
	settings = "os", "compiler", "build_type", "arch"
	requires = "boost-ext-ut/1.1.8"
	generators = "CMakeToolchain", "CMakeDeps"

	def layout(self):
		cmake_layout(self)

	def generate(self):
		toolchain = CMakeToolchain(self, generator='Ninja')
		toolchain.variables["BUILD_TESTS"] = True
		toolchain.variables["BOOST_UT_DISABLE_MODULE"] = True
		toolchain.generate()

	def build(self):
		cmake = CMake()
		cmake.configure()
		cmake.build()

	def package(self):
		cmake = CMake()
		cmake.install()