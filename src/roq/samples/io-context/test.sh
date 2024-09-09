#!/usr/bin/env bash

./roq-cpp-samples-io-context \
  --name trader \
  --ws_port 2345 \
  --udp_port 1234 \
  ~/run/deribit.sock
