load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("//:common_copts.bzl", "copts", "linkopts")

config_setting(
    name = "unit_testing",
    define_values = {"UNIT_TESTING": "true"},
)

config_setting(
    name = "benchmarking",
    define_values = {"BENCHMARKING": "true"},
)

config_setting(
    name = "optimized",
    define_values = {"OPTIMIZED": "true"},
)

config_setting(
    name = "debug",
    define_values = {"DEBUG": "true"},
)

config_setting(
    name = "profile",
    define_values = {"PROFILE": "true"},
)

config_setting(
    name = "profile_optimized",
    define_values = {"PROFILE_OPTIMIZED": "true"},
)

cc_library(
    name = "sim_framework",
    srcs = glob([
        "src/**/*.cpp",
        "include/**/*.hpp",
    ]),
    copts = copts(),
    includes = ["include"],
    linkopts = linkopts(),
    linkstatic = True,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/rttr_bazel:rttr_core",
        "@eigen",
        "@magic_enum",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    copts = copts(),
    linkopts = linkopts(),
    linkstatic = True,
    deps = [
        "//:sim_framework",
        "@cli11",
    ],
)

# Useful aliases

## Create "compile_commands.json" such that VSCode can find includes paths based on bazel pulls, not system headers
alias(
    name = "compile_commands",
    actual = "@wolfd_bazel_compile_commands//:generate_compile_commands",
)
