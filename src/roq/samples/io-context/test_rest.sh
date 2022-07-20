#!/usr/bin/env bash

curl \
  --request POST \
  --header "Content-Type: application/json" \
  --data '{"action":"subscribe","symbol":"BTC-PERPETUAL"}' \
   http://localhost:2345/test \
   --next \
  --header "Connection: close" \
  --header "Content-Type: application/json" \
  --data '{"action":"subscribe","symbol":"BTC-PERPETUAL"}' \
   http://localhost:2345/test \
   $@
