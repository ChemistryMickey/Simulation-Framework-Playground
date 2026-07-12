set shell := ["bash", "-ecu", "-o", "pipefail"]

ROOT := module_directory()
WORKDIR := invocation_directory()
rebuild_compile_commands := "true"

# List all available commands
default:
    just --list

build *args:
    {{ROOT}}/scripts/build.sh {{args}}

test *args:
    {{ROOT}}/scripts/test.sh {{ROOT}} {{args}}

# Removes all bazel-* directories. Should not really be necessary.
clean:
    bazel clean

# Removes everything including cached remote-fetched files
nuke:
    bazel clean --expunge


