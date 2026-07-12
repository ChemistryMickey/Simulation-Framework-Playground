load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

# --------------------------------------------------------------------
# Header files
# --------------------------------------------------------------------
filegroup(
    name = "headers",
    srcs = glob(
        [
            "src/rttr/**/*",
        ],
        exclude = [
            # Exclude C++ sources
            "*.cpp",
        ],
    ),
)

# --------------------------------------------------------------------
# All RTTR .cpp sources
# --------------------------------------------------------------------
rttr_srcs = glob(["src/rttr/**/*.cpp"])

# --------------------------------------------------------------------
# Common compile flags
# --------------------------------------------------------------------
rttr_copts = [
    "--std=c++23",
    # "-DRTTR_DISABLE_WARNINGS",
    # "-DRTTR_DLL",           # needed for shared lib
    # "-DRTTR_COMPILER_GCC",
    "-fPIC",
    "-O3",
    "-w",  # Suppress all warnings
    "-Wno-error",
]

# --------------------------------------------------------------------
# Static library (librttr_core.a)
# --------------------------------------------------------------------
cc_library(
    name = "rttr_core_static",
    srcs = rttr_srcs,
    hdrs = [":headers"],
    copts = rttr_copts,
    includes = ["src"],
    linkstatic = True,
)

# --------------------------------------------------------------------
# Alias: choose static by default, like CMake "rttr::rttr_core"
# --------------------------------------------------------------------
alias(
    name = "rttr_core",
    actual = ":rttr_core_static",
)
