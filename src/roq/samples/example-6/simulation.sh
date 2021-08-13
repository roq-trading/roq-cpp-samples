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

$PREFIX "./roq-samples-example-6" \
  --name "$NAME" \
  --simulation=true \
  --timer_freq=1s \
  $CONDA_PREFIX/share/roq/data/deribit.roq \
  $CONDA_PREFIX/share/roq/data/bitmex.roq \
  $@
