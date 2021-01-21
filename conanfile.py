from conans import ConanFile, CMake

class SynodicCoreConan(ConanFile):
	name = "synodic-template-library"
	version = "0.1.0"
	license = "MIT"
	homepage = "https://github.com/Synodic-Software/Synodic-Template-Library"
	settings = "os", "compiler", "build_type", "arch"
	requires = "doctest/2.4.4"
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
		self.copy("*.lib", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.so", dst="lib", keep_path=False)
		self.copy("*.dylib", dst="lib", keep_path=False)
		self.copy("*.a", dst="lib", keep_path=False)

	def package_info(self):
		self.cpp_info.libs = ["synodic-template-library"]

		# If the the package is in editable mode, forward the library path
		if not self.in_local_cache:
			self.cpp_info.includedirs = ["src"]
			self.cpp_info.libdirs = ["build"]
