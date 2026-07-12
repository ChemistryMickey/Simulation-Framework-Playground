load("@rules_cc//cc:cc_test.bzl", "cc_test")
load("//:common_copts.bzl", "copts", "linkopts")
load("//tests:generate_tests.bzl", "generate_tests")

deps = [
    "//:sim_framework",
    "//third_party/rttr_bazel:rttr_core",
    "@cli11//:cli11",
    "@boost.pfr//:boost.pfr",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
]

# Unit tests
UT_test_glob_matcher = "*.cpp"

UT_test_names = generate_tests(UT_test_glob_matcher, deps, [])

test_suite(
    name = "cached",
    tests = UT_test_names,
)

cc_test(
    name = "all",
    size = "small",
    srcs = [file for file in glob(
        [
            UT_test_glob_matcher,
        ],
        allow_empty = True,
    )],
    copts = copts(),
    linkopts = linkopts(),
)
