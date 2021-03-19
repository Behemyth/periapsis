from conans import ConanFile, CMake

class SynodicCoreConan(ConanFile):
	name = "synodic-template-library"
	version = "0.1.0"
	license = "MIT"
	homepage = "https://github.com/Synodic-Software/Synodic-Template-Library"
	settings = "os", "compiler", "build_type", "arch"
	requires = "boost-ext-ut/1.1.8"
	generators = "cmake_find_package"
	scm = {
         "type": "git",
         "url": "auto",
         "revision": "auto",
    }

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		self.copy("*.h", src="src", dst="include")

	def package_id(self):
		self.info.header_only()

	def package_info(self):

		# If the the package is in editable mode, forward the library path
		if not self.in_local_cache:
			self.cpp_info.includedirs = ["src"]