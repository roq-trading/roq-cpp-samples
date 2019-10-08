#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="trader"

EXCHANGE="coinbase-pro"
ACCOUNTS="A1"
SYMBOLS="BTC-USD"

$PREFIX "$DIR/roq-samples-simple" \
	--name "$NAME" \
	--dispatcher-affinity 0 \
	--metrics "$(realpath "$DIR/coinbase-pro_metrics.sock")" \
	--exchange "$EXCHANGE" \
	--accounts "$ACCOUNTS" \
	--symbols "$SYMBOLS" \
	$@
