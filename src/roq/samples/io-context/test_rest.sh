#!/usr/bin/env bash

curl \
  --request GET \
  --header "Content-Type: application/json" \
  --data '{"action":"subscribe","symbol":"BTC-PERPETUAL"}' \
   http://localhost:2345/test \
   $@
