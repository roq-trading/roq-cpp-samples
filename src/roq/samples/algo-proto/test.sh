#!/usr/bin/env bash

CONFIG_FILE="config.toml"

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

$PREFIX "./roq-cpp-samples-algo-proto" \
  --name "trader" \
  --config_file "$CONFIG_FILE" \
  --strategy "spread_btc" \
  --side BUY \
  --quantity 1 \
  --price 100 \
  $@
