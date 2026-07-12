load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("//:common_copts.bzl", "copts", "linkopts")

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    copts = copts(),
    linkopts = linkopts(),
    linkstatic = True,
    deps = [
        "//sim_framework",
        "@cli11",
        "@eigen",
    ],
)
