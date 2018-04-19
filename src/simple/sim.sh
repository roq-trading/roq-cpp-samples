#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	shift 1
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./example-simple \
	--config-file simple.conf \
	--simulation-file ./USTP_Multi_20180223.csv $@
