#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
else
  PREFIX=
fi

NAME="collector"

SERVER1="server-1"
SERVER2="server-2"

$PREFIX "$DIR/roq-samples-collector" \
	--name "$NAME" \
	--dispatcher-affinity 1 \
	--metrics "$(realpath "$DIR/${NAME}_metrics.sock")" \
	"$(realpath "$DIR/../$SERVER1.sock")" \
	"$(realpath "$DIR/../$SERVER2.sock")" \
	$@
