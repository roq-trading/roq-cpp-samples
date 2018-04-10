#!/usr/bin/env bash

if [ "$1" == "debug" ]; then
	PREFIX="libtool --mode=execute gdb --args"
else
	PREFIX=
fi

$PREFIX ./example-simple \
	--config-file simple.conf \
	--config-variables instruments.conf \
	--gateways femas=test:1234@/var/tmp/femasapi.sock
