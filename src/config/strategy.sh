#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	shift 1
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./example-config --strategy ~/gqstrats01IC.ini
