#!/usr/bin/env bash

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

$PREFIX "./roq-cpp-samples-event-log" \
  $CONDA_PREFIX/share/roq/data/deribit.roq \
  $@
