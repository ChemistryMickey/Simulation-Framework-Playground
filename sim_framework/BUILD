load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("//:common_copts.bzl", "copts", "linkopts")

cc_library(
    name = "sim_framework",
    srcs = glob([
        "src/**/*.cpp",
    ]),
    hdrs = glob(["include/**/*.hpp"]),
    copts = copts(),
    linkopts = linkopts(),
    linkstatic = True,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/rttr_bazel:rttr_core",
        "@eigen",
        "@magic_enum",
        "@yaml-cpp",
    ],
)
