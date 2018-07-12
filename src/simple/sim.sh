#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	shift 1
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./roq-samples-simple \
	--config-file simple.conf \
	--simulation-file ./femasapi_2018-07-11.csv $@
