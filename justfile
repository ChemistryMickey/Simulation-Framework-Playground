set shell := ["bash", "-eu", "-o", "pipefail", "-c"]

ROOT := module_directory()
WORKDIR := invocation_directory()
clean := "false"
rebuild_compile_commands := "true"

# List all available commands
default:
    just --list
