import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import can_run

def get_latest_tag():
    try:
        # Get the latest tag using os.popen and os.system
        latest_tag = os.popen("git describe --tags `git rev-list --tags --max-count=1`").read().strip()
        if not latest_tag:
            return "1.0.0"  # Fallback version if no tags are found
        return latest_tag
    except Exception as e:
        return "1.0.0"  # Fallback version in case of an error

class AstragaramLibs(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   name = "astragaram-libs"
   version = get_latest_tag()

   def build_requirements(self):
      self.tool_requires("cmake/3.30.5")
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

   def layout(self):
        cmake_layout(self)
