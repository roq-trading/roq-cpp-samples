#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ "$1" == "debug" ]; then
  PREFIX="gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="trader"

$PREFIX "$DIR/roq-samples-example-2" \
  --name "$NAME" \
  $@
