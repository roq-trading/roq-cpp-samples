#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="simple"

SERVER1="server-1"
SERVER2="server-2"

$PREFIX "$DIR/roq-samples-simple" \
	--name "$NAME" \
	--dispatcher-affinity 0 \
	--metrics "$(realpath "$DIR/${NAME}_metrics.sock")" \
	"$(realpath "$DIR/../$SERVER1.sock")" \
	"$(realpath "$DIR/../$SERVER2.sock")" \
	$@
