#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
else
  PREFIX=
fi

NAME="collector"

$PREFIX "$DIR/roq-samples-collector" \
	--name "$NAME" \
	--dispatcher-affinity 1 \
	--metrics "$(realpath "$DIR/${NAME}_metrics.sock")" \
	$@
