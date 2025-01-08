import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import can_run


def get_latest_tag():
    try:
        # Get the latest tag using os.popen and os.system
        latest_tag = (
            os.popen("git describe --tags `git rev-list --tags --max-count=1`")
            .read()
            .strip()
        )
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

    def run_astyle(self):
        astyle_path = self.which("astyle")
        if astyle_path:
            self.output.info("Running astyle to format source files")
            os.chdir(self.source_folder)  # Folder change to Source
            os.system(
                'find . -name "*.cpp" -o -name "*.h" | xargs astyle --options=none --indent=spaces=4 --style=break --indent-preprocessor --pad-oper --pad-paren-in  --unpad-paren --convert-tabs --preserve-date --formatted --min-conditional-indent=0 --max-instatement-indent=80 --lineend=linux --suffix=none'
            )
            os.chdir(self.build_folder)  # Change back to build folder
        else:
            self.output.info(
                "astyle is not installed. Skipping code formatting. Suggesting to install astyle: sudo apt-get install -y astyle"
            )

    def build(self):
        self.run_astyle()
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        # Run all tests using ctest
        self.run("ctest --output-on-failure")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def layout(self):
        cmake_layout(self)

    def which(self, program):
        import shutil

        return shutil.which(program)
