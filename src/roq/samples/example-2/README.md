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
I0724 16:21:24.140121 24078 application.cpp:42] ===== START =====
I0724 16:21:24.140762 24078 service.cpp:39] The metrics service will *not* be started
I0724 16:21:24.146542 24078 controller.cpp:108] Dispatching...
I0724 16:21:24.146591 24078 controller.cpp:112] Starting event loop thread...
I0724 16:21:24.146925 24079 controller.cpp:148] Event loop thread is now running
I0724 16:21:25.146982 24079 session_manager.cpp:44] Connecting "unix:///home/thraneh/coinbase-pro.sock"
I0724 16:21:25.147074 24079 session_manager.cpp:44] Connecting "unix:///home/thraneh/deribit.sock"
I0724 16:21:25.147198 24079 session.cpp:38] Adding name="coinbase-pro" (user_id=2)
I0724 16:21:25.147262 24078 pollster.cpp:403] Adding name="coinbase-pro" (user_id=2)
I0724 16:21:25.147270 24079 session.cpp:38] Adding name="deribit" (user_id=2)
I0724 16:21:25.147282 24078 main.cpp:126] [coinbase-pro:BTC-USD] connection_status=CONNECTED
I0724 16:21:25.147301 24078 pollster.cpp:403] Adding name="deribit" (user_id=2)
I0724 16:21:25.147310 24078 main.cpp:126] [deribit:BTC-PERPETUAL] connection_status=CONNECTED
I0724 16:21:25.147397 24078 main.cpp:152] [coinbase-pro:BTC-USD] download=true
I0724 16:21:25.147403 24078 main.cpp:176] [coinbase-pro:BTC-USD] market_data_status=READY
I0724 16:21:25.147413 24078 main.cpp:193] [coinbase-pro:BTC-USD] tick_size=0.010000000000000002
I0724 16:21:25.147419 24078 main.cpp:200] [coinbase-pro:BTC-USD] min_trade_vol=0.0010000000000000002
I0724 16:21:25.147423 24078 main.cpp:207] [coinbase-pro:BTC-USD] multiplier=1.0
I0724 16:21:25.147428 24078 main.cpp:222] [coinbase-pro:BTC-USD] trading_status=OPEN
I0724 16:21:25.147477 24078 main.cpp:235] MarketByPriceUpdate={exchange="coinbase-pro", symbol="BTC-USD", ...
I0724 16:21:25.147532 24078 main.cpp:164] [coinbase-pro:BTC-USD] download=false
I0724 16:21:25.147537 24078 main.cpp:328] [coinbase-pro:BTC-USD] ready=true
I0724 16:21:25.147547 24078 main.cpp:152] [deribit:BTC-PERPETUAL] download=true
I0724 16:21:25.147551 24078 main.cpp:176] [deribit:BTC-PERPETUAL] market_data_status=READY
I0724 16:21:25.147559 24078 main.cpp:193] [deribit:BTC-PERPETUAL] tick_size=0.5
I0724 16:21:25.147562 24078 main.cpp:200] [deribit:BTC-PERPETUAL] min_trade_vol=1.0
I0724 16:21:25.147565 24078 main.cpp:207] [deribit:BTC-PERPETUAL] multiplier=10.0
I0724 16:21:25.147739 24078 main.cpp:222] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0724 16:21:25.148082 24078 main.cpp:235] MarketByPriceUpdate={exchange="deribit", symbol="BTC-PERPETUAL", ...
I0724 16:21:25.148250 24078 main.cpp:164] [deribit:BTC-PERPETUAL] download=false
I0724 16:21:25.148254 24078 main.cpp:328] [deribit:BTC-PERPETUAL] ready=true
```

Note! The MarketByPrice updates have been truncated for readability
