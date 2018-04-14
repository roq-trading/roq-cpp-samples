#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./example-spread \
	--config-file spread.conf \
	--simulation-file ./USTP_Multi_20180223.csv
