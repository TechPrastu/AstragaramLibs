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
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    name = "astragaram-libs"
    author = "Jigneshkumar Vadaviya"
    url = "https://github.com/TechPrastu/AstragaramLibs"
    version = get_latest_tag()
    scm = { 
        "type": "git",
        "url": "git@github.com:TechPrastu/AstragaramLibs.git",
        "revision": "auto", 
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = [
        "CMakeLists.txt",
        "*",
        "!build/*",
        "!.vsconan/*",
        "!.vscode/*",
        "!*.d",
        "!*.slo",
        "!*.lo",
        "!*.o",
        "!*.obj",
        "!*.gch",
        "!*.pch",
        "!*.so",
        "!*.dylib",
        "!*.dll",
        "!*.mod",
        "!*.smod",
        "!*.lai",
        "!*.la",
        "!*.a",
        "!*.lib",
        "!*.exe",
        "!*.out",
        "!*.app",
        "!CMakeUserPresets.json"
    ]

    def build_requirements(self):
        self.tool_requires("cmake/3.30.5")
        self.requires("gtest/1.15.0")
        self.requires("protobuf/3.18.1")
        self.requires("boost/1.81.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def run_astyle(self):
        astyle_path = self.which("astyle")
        if astyle_path:
            self.output.info("Running astyle to format source files")
            os.chdir(self.source_folder)  # Change to Source folder
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
        cmake.verbose = True  # Enable verbose output
        cmake.build()
        # Run tests and check the return code
        try:
            cmake.test()
        except Exception as e:
            self.output.error("Initial test run failed. Rerunning failed tests with detailed output.")
            self.run("ctest --rerun-failed --output-on-failure")
            raise 

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def layout(self):
        cmake_layout(self)

    def which(self, program):
        import shutil
        return shutil.which(program)
