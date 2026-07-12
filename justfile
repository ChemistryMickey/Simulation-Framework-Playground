set shell := ["bash", "-ecu", "-o", "pipefail"]

ROOT := module_directory()
WORKDIR := invocation_directory()
rebuild_compile_commands := "true"

# List all available commands
default:
    just --list

build *args: compile_commands
    {{ROOT}}/scripts/build.sh {{args}}

test *args: compile_commands
    {{ROOT}}/scripts/test.sh {{ROOT}} {{args}}

compile_commands:
    #! /usr/bin/bash
    if [[ {{rebuild_compile_commands}} == "true" ]]; then
        bazel run compile_commands
    fi

# Removes all bazel-* directories. Should not really be necessary.
clean:
    bazel clean

# Removes everything including cached remote-fetched files
nuke:
    bazel clean --expunge


