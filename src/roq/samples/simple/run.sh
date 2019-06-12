#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="simple"

EXCHANGE="CFFEX"
ACCOUNTS="A1"
SYMBOLS="IC1803"

$PREFIX "$DIR/roq-samples-simple" \
	--name "$NAME" \
	--dispatcher-affinity 0 \
	--metrics "$(realpath "$DIR/${NAME}_metrics.sock")" \
	--exchange "$EXCHANGE" \
	--accounts "$ACCOUNTS" \
	--symbols "$SYMBOLS" \
	$@
