#! /usr/bin/bash

target="cached"
if [[ $# == 2 ]]; then
	target="$2"
	for t in $(ls $1/tests/*$target*.cpp); do
		bazel test //tests:$(basename $t ".cpp") --config=unit_testing -j $(nproc)
	done
else
	bazel test //tests:$target --config=unit_testing -j $(nproc)
fi
