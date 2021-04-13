# Example 2

Demonstrates how to maintain state and update a simple model.

* Extends `example-1`
* Cache instrument specific information (such as tick size)
* Process MarketByPrice and maintain a view of depth
* Compute weighted average price and exponential moving average

## Prerequisites

Please refer [here](../../../../README.md#gateways-install-configure-run) to
install, configure and run the required market gateways.


## Using

```bash
./roq-samples-example-2 \
    --name "trader" \
    ~/deribit.sock \
    ~/coinbase-pro.sock
```

```text
I0413 16:04:55.321692 19803971 service.cpp:50] ===== START =====
I0413 16:04:55.322590 19803971 service.cpp:34] The metrics service will *not* be started
I0413 16:04:55.332823 19803971 controller.cpp:74] session_id="a619015b-e720-48a7-9580-e25b49c80a15"
I0413 16:04:55.332840 19803971 controller.cpp:78] Dispatching...
I0413 16:04:55.332892 19803971 controller.cpp:82] Starting event loop thread...
I0413 16:04:55.333617 19804165 controller.cpp:113] Event loop thread is now running
I0413 16:04:56.333282 19804165 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-deribit/src/roq/deribit/deribit-test.sock"
I0413 16:04:56.333534 19804165 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-coinbase-pro/src/roq/coinbase_pro/coinbase-pro-sandbox.sock"
I0413 16:04:56.334276 19804165 session.cpp:32] Adding name="coinbase-pro" (user_id=1)
I0413 16:04:56.334512 19804165 session.cpp:32] Adding name="deribit" (user_id=1)
I0413 16:04:56.334487 19803971 pollster.cpp:305] Adding name="coinbase-pro" (user_id=1)
I0413 16:04:56.336018 19803971 instrument.cpp:27] [coinbase-pro:BTC-USD] connected=true
I0413 16:04:56.336049 19803971 pollster.cpp:305] Adding name="deribit" (user_id=1)
I0413 16:04:56.336079 19803971 instrument.cpp:27] [deribit:BTC-PERPETUAL] connected=true
I0413 16:04:56.336669 19803971 instrument.cpp:45] [deribit:BTC-PERPETUAL] download=true
I0413 16:04:56.336873 19803971 instrument.cpp:73] [deribit:BTC-PERPETUAL] market_data=true
I0413 16:04:56.340776 19803971 instrument.cpp:90] [deribit:BTC-PERPETUAL] tick_size=0.5
I0413 16:04:56.340813 19803971 instrument.cpp:93] [deribit:BTC-PERPETUAL] min_trade_vol=1
I0413 16:04:56.340823 19803971 instrument.cpp:96] [deribit:BTC-PERPETUAL] multiplier=10
I0413 16:04:56.340839 19803971 instrument.cpp:45] [coinbase-pro:BTC-USD] download=true
I0413 16:04:56.340882 19803971 instrument.cpp:73] [coinbase-pro:BTC-USD] market_data=true
I0413 16:04:56.341208 19803971 instrument.cpp:90] [coinbase-pro:BTC-USD] tick_size=0.010000000000000002
I0413 16:04:56.341281 19803971 instrument.cpp:93] [coinbase-pro:BTC-USD] min_trade_vol=0.0010000000000000002
I0413 16:04:56.341305 19803971 instrument.cpp:107] [coinbase-pro:BTC-USD] trading_status=OPEN
I0413 16:04:56.342191 19803971 instrument.cpp:117] MarketByPriceUpdate={stream_id=4, exchange="coinbase-pro", symbol="BTC-USD", ...
I0413 16:04:56.364501 19803971 instrument.cpp:53] [coinbase-pro:BTC-USD] download=false
I0413 16:04:56.364548 19803971 instrument.cpp:107] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0413 16:04:56.365014 19803971 instrument.cpp:117] MarketByPriceUpdate={stream_id=4, exchange="deribit", symbol="BTC-PERPETUAL", ...
I0413 16:04:56.386994 19803971 instrument.cpp:53] [deribit:BTC-PERPETUAL] download=false
I0413 16:04:56.387006 19803971 instrument.cpp:193] [deribit:BTC-PERPETUAL] ready=true
```

Note! The MarketByPrice updates have been truncated for readability
