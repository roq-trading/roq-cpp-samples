#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
else
  PREFIX=
fi

NAME="taker"

HISTORY1="./history1"
HISTORY2="./history2"

$PREFIX "$DIR/roq-samples-taker" \
	--name "$NAME" \
	--simulation \
	"$HISTORY1" \
	"$HISTORY2" \
	$@
