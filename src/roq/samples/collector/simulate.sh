#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="libtool --mode=execute gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="collector"

HISTORY="$CONDA_PREFIX/share/roq/data/USTP_Multi_20180223.csv"

$PREFIX "$DIR/roq-samples-collector" \
	--name "$NAME" \
	--simulation \
	--generator-type "csv-old" \
	"$HISTORY" \
	$@
