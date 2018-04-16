#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	shift 1
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./example-config \
	--strategy-name GQIF \
	--strategy-group ~/strategy_gxu_group.json
