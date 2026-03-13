from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import copy
import os

class IvshmConan(ConanFile):
    name = "ivshm"
    license = "Apache 2.0"
    author = "lanmeng <pineconeforlanm@outlook.com>"
    url = "https://gitee.com/pinecone-songguo/ivshm.git"
    description = "A high-performance IVSHM library with gRPC support"
    topics = ("ivshm", "cpp23")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_coverage": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "enable_coverage": False
    }

    def set_version(self):
        git = Git(self, folder=self.recipe_folder)
        try:
            tag = git.run("describe --tags --abbrev=0")
            self.version = tag.strip().lstrip('v')
        except Exception:
            self.version = "0.1.0-dev"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("boost/1.89.0", force=True)
        self.requires("magic_enum/0.9.7")

        if self.settings.os == "Linux":
            self.requires("liburing/2.13")

        # 性能测试与单元测试依赖
        self.test_requires("gtest/1.17.0")
        self.test_requires("benchmark/1.9.4")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)

        git = Git(self, folder=self.export_sources_folder)
        try:
            commit = git.get_commit()
            branch = git.get_branch()
        except:
            commit = "unknown"
            branch = "unknown"

        # 传递给 CMake 的变量
        tc.variables["PROJECT_LATEST_COMMIT_HASH"] = commit
        tc.variables["PROJECT_BRANCH_NAME"] = branch
        tc.variables["ENABLE_COVERAGE"] = self.options.enable_coverage
        tc.variables["YLT_ENABLE_IO_URING"] = "ON"
        tc.variables["YLT_ENABLE_SSL"] = "ON"

        tc.cache_variables["CMAKE_POLICY_VERSION_MINIMUM"] = "3.5"

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        # 如果需要手动拷贝特定的文件
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))

    def package_info(self):
        # 告诉使用者如何链接你的库
        self.cpp_info.libs = ["ivshm"]
        if self.options.shared:
            self.cpp_info.defines.append("IVSHM_IS_DLL")
