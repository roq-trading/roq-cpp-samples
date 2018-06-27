#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	shift 1
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./roq-samples-simple \
	--config-file simple.conf \
	--gateways femas=trader:secret@/var/tmp/femasapi.sock $@
