from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class FgConan(ConanFile):
    name            = "fg"
    version         = "1.1.0"					
    description     = "Conan package for fg."												 
    url             = "https://github.com/acdemiralp/fg"
    license         = "MIT"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake = CMake(self)
        self.run("cmake %s-%s %s" % (self.name, self.version, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
