#!/usr/bin/env bash

./roq-cpp-samples-strategy-1 \
  --name "trader" \
  --account "A1" \
  --exchange "deribit" \
  --symbol "BTC-PERPETUAL" \
  --quantity 1 \
  --price_low 50000 \
  --price_high 150000 \
  --max_retries 3 \
  --retry_delay "10s" \
  $@
