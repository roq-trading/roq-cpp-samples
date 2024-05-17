# Example 3

Demonstrates how to place limit orders based on model signal.

* Extends `example-2`
* Keeps track of order updates and maintain long/short positions
* Places limit orders, subject to position, when the model generates a signal

The model tries to detect sharp moves (perhaps significant order flow) and will
generate a signal when the directional move reverts.

> The model is very simplistic and the parameters have not been optimized:
> do **not** use this for any real trading!


## Prerequisites

Please refer [here](../../../../README.md#gateways-install-configure-run) to
install, configure and run the required market gateways.


## Using

Historical simulation (back-testing) is very useful to verify the performance of
a trading strategy

For this example, make sure you have installed the `roq-data` conda package.

### Simulation

Simulation is controlled by the `--simulation=true` flag

```bash
./roq-samples-example-3 \
    --name "trader" \
    --simulation=true \
    $CONDA_PREFIX/share/roq/data/deribit.roq
```

```text
I0223 12:25:05.530707 4468548 service.cpp:46] ===== START =====
I0223 12:25:05.530830 4468548 service.cpp:47] package name  : roq-samples
I0223 12:25:05.530833 4468548 service.cpp:48] build version : 0.8.2
I0223 12:25:05.530835 4468548 service.cpp:49] build number  : 
I0223 12:25:05.530838 4468548 service.cpp:50] build type    : 
I0223 12:25:05.530840 4468548 service.cpp:51] git hash      : 
I0223 12:25:05.530844 4468548 service.cpp:52] compile time  : Feb 23 2022 09:40:10
I0223 12:25:05.571468 4468548 instrument.cpp:44] [deribit:BTC-PERPETUAL] connected=true
I0223 12:25:05.571530 4468548 instrument.cpp:109] DEBUG: missing=0x550000
I0223 12:25:05.571537 4468548 strategy.cpp:50] max_order_id=1000
I0223 12:25:05.571553 4468548 instrument.cpp:62] [deribit:BTC-PERPETUAL] download=true
I0223 12:25:05.571556 4468548 instrument.cpp:92] DEBUG: missing=0xb
I0223 12:25:05.571559 4468548 instrument.cpp:70] [deribit:BTC-PERPETUAL] download=false
I0223 12:25:05.571570 4468548 instrument.cpp:109] DEBUG: missing=0x550000
I0223 12:25:05.571580 4468548 instrument.cpp:92] DEBUG: missing=0xb
I0223 12:25:05.571587 4468548 instrument.cpp:105] [deribit:BTC-PERPETUAL] order_management=true
I0223 12:25:05.571590 4468548 instrument.cpp:109] DEBUG: missing=0x0
I0223 12:25:05.571600 4468548 instrument.cpp:109] DEBUG: missing=0x0
I0223 12:25:05.578785 4468548 instrument.cpp:121] [deribit:BTC-PERPETUAL] tick_size=0.5
I0223 12:25:05.578792 4468548 instrument.cpp:124] [deribit:BTC-PERPETUAL] min_trade_vol=1
I0223 12:25:05.578795 4468548 instrument.cpp:127] [deribit:BTC-PERPETUAL] multiplier=10
I0223 12:25:05.579377 4468548 instrument.cpp:92] DEBUG: missing=0x2
I0223 12:25:05.579388 4468548 instrument.cpp:88] [deribit:BTC-PERPETUAL] market_data=true
I0223 12:25:05.579399 4468548 instrument.cpp:88] [deribit:BTC-PERPETUAL] market_data=false
I0223 12:25:05.579402 4468548 instrument.cpp:92] DEBUG: missing=0x8
I0223 12:25:05.581358 4468548 instrument.cpp:88] [deribit:BTC-PERPETUAL] market_data=true
I0223 12:25:05.582653 4468548 instrument.cpp:138] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0223 12:25:05.582657 4468548 instrument.cpp:203] [deribit:BTC-PERPETUAL] ready=true
I0223 12:25:05.682896 4468548 model.cpp:50] DIRECTION: SELLING
I0223 12:25:05.695562 4468548 model.cpp:64] DIRECTION: BUYING
I0223 12:25:05.795545 4468548 model.cpp:58] SIGNAL: SELL @ 38887.5
W0223 12:25:05.795552 4468548 strategy.cpp:121] Trading *NOT* enabled
I0223 12:25:05.862908 4468548 model.cpp:50] DIRECTION: SELLING
I0223 12:25:05.888680 4468548 model.cpp:44] SIGNAL: BUY @ 38881.5
W0223 12:25:05.888684 4468548 strategy.cpp:121] Trading *NOT* enabled
I0223 12:25:05.906430 4468548 model.cpp:64] DIRECTION: BUYING
I0223 12:25:05.907219 4468548 model.cpp:58] SIGNAL: SELL @ 38891.5
W0223 12:25:05.907223 4468548 strategy.cpp:121] Trading *NOT* enabled
...
I0223 12:25:06.413306 4468548 model.cpp:64] DIRECTION: BUYING
I0223 12:25:06.414215 4468548 model.cpp:58] SIGNAL: SELL @ 38938
W0223 12:25:06.414222 4468548 strategy.cpp:121] Trading *NOT* enabled
I0223 12:25:06.440627 4468548 model.cpp:50] DIRECTION: SELLING
I0223 12:25:06.454401 4468548 instrument.cpp:51] [deribit:BTC-PERPETUAL] connected=false
I0223 12:25:06.526573 4468548 service.cpp:65] ===== STOP =====
```

### Order Management

Placement of limit orders is controlled by the `--enable_trading` flag

```bash
./roq-samples-example-3 \
    --name "trader" \
    --simulation=true \
    --enable_trading \
    $CONDA_PREFIX/share/roq/data/deribit.roq
```

```text
I0223 12:26:02.355940 4473209 service.cpp:46] ===== START =====
I0223 12:26:02.356096 4473209 service.cpp:47] package name  : roq-samples
I0223 12:26:02.356099 4473209 service.cpp:48] build version : 0.8.2
I0223 12:26:02.356102 4473209 service.cpp:49] build number  : 
I0223 12:26:02.356104 4473209 service.cpp:50] build type    : 
I0223 12:26:02.356106 4473209 service.cpp:51] git hash      : 
I0223 12:26:02.356111 4473209 service.cpp:52] compile time  : Feb 23 2022 09:40:10
I0223 12:26:02.398613 4473209 instrument.cpp:44] [deribit:BTC-PERPETUAL] connected=true
I0223 12:26:02.398688 4473209 instrument.cpp:109] DEBUG: missing=0x550000
I0223 12:26:02.398692 4473209 strategy.cpp:50] max_order_id=1000
I0223 12:26:02.398708 4473209 instrument.cpp:62] [deribit:BTC-PERPETUAL] download=true
I0223 12:26:02.398711 4473209 instrument.cpp:92] DEBUG: missing=0xb
I0223 12:26:02.398714 4473209 instrument.cpp:70] [deribit:BTC-PERPETUAL] download=false
I0223 12:26:02.398725 4473209 instrument.cpp:109] DEBUG: missing=0x550000
I0223 12:26:02.398735 4473209 instrument.cpp:92] DEBUG: missing=0xb
I0223 12:26:02.398742 4473209 instrument.cpp:105] [deribit:BTC-PERPETUAL] order_management=true
I0223 12:26:02.398745 4473209 instrument.cpp:109] DEBUG: missing=0x0
I0223 12:26:02.398754 4473209 instrument.cpp:109] DEBUG: missing=0x0
I0223 12:26:02.406340 4473209 instrument.cpp:121] [deribit:BTC-PERPETUAL] tick_size=0.5
I0223 12:26:02.406354 4473209 instrument.cpp:124] [deribit:BTC-PERPETUAL] min_trade_vol=1
I0223 12:26:02.406357 4473209 instrument.cpp:127] [deribit:BTC-PERPETUAL] multiplier=10
I0223 12:26:02.406974 4473209 instrument.cpp:92] DEBUG: missing=0x2
I0223 12:26:02.406988 4473209 instrument.cpp:88] [deribit:BTC-PERPETUAL] market_data=true
I0223 12:26:02.406999 4473209 instrument.cpp:88] [deribit:BTC-PERPETUAL] market_data=false
I0223 12:26:02.407003 4473209 instrument.cpp:92] DEBUG: missing=0x8
I0223 12:26:02.409072 4473209 instrument.cpp:88] [deribit:BTC-PERPETUAL] market_data=true
I0223 12:26:02.410456 4473209 instrument.cpp:138] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0223 12:26:02.410463 4473209 instrument.cpp:203] [deribit:BTC-PERPETUAL] ready=true
I0223 12:26:02.516000 4473209 model.cpp:50] DIRECTION: SELLING
I0223 12:26:02.529352 4473209 model.cpp:64] DIRECTION: BUYING
I0223 12:26:02.635025 4473209 model.cpp:58] SIGNAL: SELL @ 38887.5
I0223 12:26:02.635113 4473209 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, type=CREATE_ORDER, origin=GATEWAY, status=FORWARDED, error=UNDEFINED, text="", request_id="", external_account="", external_order_id="", routing_id="", version=1, round_trip_latency=0ns}
I0223 12:26:02.635176 4473209 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", request_id="", external_account="", external_order_id="", routing_id="", version=1, round_trip_latency=0ns}
I0223 12:26:02.635209 4473209 strategy.cpp:79] OrderUpdate={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, max_show_quantity=nan, order_type=LIMIT, time_in_force=GTC, execution_instruction=UNDEFINED, order_template="", create_time_utc=1645612339452067619ns, update_time_utc=1645612339452067619ns, external_account="", external_order_id="", status=SENT, quantity=1, price=38888, stop_price=nan, remaining_quantity=1, traded_quantity=0, average_traded_price=nan, last_traded_quantity=nan, last_traded_price=nan, last_liquidity=UNDEFINED, routing_id="", max_request_version=1, max_response_version=1, max_accepted_version=1, update_type=UNDEFINED}
I0223 12:26:02.635217 4473209 instrument.cpp:182] [deribit:BTC-PERPETUAL] position=0
I0223 12:26:02.668950 4473209 strategy.cpp:89] TradeUpdate={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, create_time_utc=1645612366153860492ns, update_time_utc=1645612366153860492ns, external_account="", external_order_id="", fills=[{external_trade_id="", quantity=1, price=38888, liquidity=MAKER}], routing_id="", update_type=UNDEFINED}
I0223 12:26:02.668988 4473209 strategy.cpp:79] OrderUpdate={stream_id=0, account="A1", order_id=1001, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, max_show_quantity=nan, order_type=LIMIT, time_in_force=GTC, execution_instruction=UNDEFINED, order_template="", create_time_utc=1645612339452067619ns, update_time_utc=1645612339452067619ns, external_account="", external_order_id="", status=COMPLETED, quantity=1, price=38888, stop_price=nan, remaining_quantity=0, traded_quantity=1, average_traded_price=38888, last_traded_quantity=1, last_traded_price=38888, last_liquidity=MAKER, routing_id="", max_request_version=1, max_response_version=1, max_accepted_version=1, update_type=UNDEFINED}
I0223 12:26:02.668996 4473209 instrument.cpp:182] [deribit:BTC-PERPETUAL] position=-1
I0223 12:26:02.706309 4473209 model.cpp:50] DIRECTION: SELLING
I0223 12:26:02.733468 4473209 model.cpp:44] SIGNAL: BUY @ 38881.5
...
```

### Live Trading

Switching to live trading

```bash
./roq-samples-example-3 \
    --name "trader" \
    ~/deribit.sock
```

Then add the `--enable_trading` flag if you really want orders to be placed on
the market.
