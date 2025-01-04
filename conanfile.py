import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import can_run

class AstragaramLibs(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   name = "astragaram-libs"
   version = "1.0"

   def build_requirements(self):
      self.tool_requires("cmake/3.22.6")
      self.test_requires("gtest/1.11.0")

   def generate(self):
      tc = CMakeToolchain(self)
      tc.generate()
      deps = CMakeDeps(self)
      deps.generate()

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
      self.run(os.path.join(self.cpp.build.bindir, "test_sum"))
   
   def test(self):
        cmake = CMake(self)
        cmake.test()

   def layout(self):
        cmake_layout(self)
