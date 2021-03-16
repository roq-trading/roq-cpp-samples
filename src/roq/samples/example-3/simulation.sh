#!/usr/bin/env bash

NAME="trader"

# debug?

if [ "$1" == "debug" ]; then
  KERNEL="$(uname -a)"
  case "$KERNEL" in
    Linux*)
      PREFIX="gdb --args"
      ;;
    Darwin*)
      PREFIX="lldb --"
      ;;
  esac
  shift 1
else
  PREFIX=
fi

# launch

$PREFIX "./roq-samples-example-3" \
  --name "$NAME" \
  --simulation \
  $CONDA_PREFIX/share/roq/data/deribit.roq \
  $@
