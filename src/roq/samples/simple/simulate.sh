#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="simple"

# HISTORY="./history.csv"
HISTORY="./history_2018-02-23.csv"

$PREFIX "$DIR/roq-samples-simple" \
	--name "$NAME" \
	--exchange "CFFEX" \
	--accounts "A1" \
	--symbols "IC1803" \
	--create-orders 1 \
	--simulation \
	--generator-type "csv-old" \
	--market-data-latency 10 \
	--order-manager-latency 100 \
	"$HISTORY" \
	$@
