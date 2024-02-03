from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class AstragaramLibs(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   name = "astragaram-libs"
   version = "1.0"

   def build_requirements(self):
      self.tool_requires("cmake/3.22.6")

   def generate(self):
      tc = CMakeToolchain(self)
      tc.generate()
      deps = CMakeDeps(self)
      deps.generate()

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
