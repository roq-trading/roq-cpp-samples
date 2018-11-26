#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	shift 1
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./roq-samples-collector \
	--gateways femas=test:1234@/var/tmp/femas.sock $@
