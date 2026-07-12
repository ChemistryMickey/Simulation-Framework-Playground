#! /usr/bin/bash

target="cached"
if [[ $# == 2 ]]; then
	target="$2"
	for t in $(ls $1/sim_framework/tests/*$target*.cpp); do
		bazel test //sim_framework/tests:$(basename $t ".cpp") --config=unit_testing -j $(nproc)
	done
else
	bazel test //sim_framework/tests:$target --config=unit_testing -j $(nproc)
fi
