#!/usr/bin/env bash
set -euo pipefail

optimize=""
verbose=""
while [[ $# -ne 0 ]]; do
    case "$1" in
        --release|-r)
            optimize="--config=optimized"
            shift 1
            ;;
        --verbose|-v)
            verbose="--subcommands"
            shift 1
            ;;
        *)
            echo "Unrecognized command! $1"
            exit 1
            ;;
    esac
done

# Now print explicitly what you're doing
set -x

bazel run $verbose //src:main $optimize