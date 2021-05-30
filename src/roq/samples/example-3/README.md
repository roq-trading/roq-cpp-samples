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

Simulation is controlled by the `--simulation` flag

```bash
./roq-samples-example-3 \
    --name "trader" \
    --simulation \
    $CONDA_PREFIX/share/roq/data/deribit.roq
```

```text
I0413 16:06:47.800719 19807506 service.cpp:50] ===== START =====
I0413 16:06:47.876978 19807506 instrument.cpp:47] [deribit:BTC-PERPETUAL] connected=true
I0413 16:06:47.878850 19807506 instrument.cpp:109] [deribit:BTC-PERPETUAL] order_management=true
I0413 16:06:47.880525 19807506 instrument.cpp:144] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0413 16:06:47.931887 19807506 instrument.cpp:127] [deribit:BTC-PERPETUAL] tick_size=0.5
I0413 16:06:47.931904 19807506 instrument.cpp:130] [deribit:BTC-PERPETUAL] min_trade_vol=1
I0413 16:06:47.931908 19807506 instrument.cpp:133] [deribit:BTC-PERPETUAL] multiplier=10
I0413 16:06:47.933790 19807506 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=true
I0413 16:06:47.933802 19807506 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=true
I0413 16:06:47.934528 19807506 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=false
I0413 16:06:47.934543 19807506 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=false
I0413 16:06:47.934583 19807506 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=true
I0413 16:06:47.934587 19807506 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=true
I0413 16:06:48.607114 19807506 model.cpp:50] DIRECTION: SELLING
I0413 16:06:48.622758 19807506 model.cpp:64] DIRECTION: BUYING
I0413 16:06:48.639687 19807506 model.cpp:50] DIRECTION: SELLING
I0413 16:06:48.711781 19807506 model.cpp:64] DIRECTION: BUYING
I0413 16:06:48.761409 19807506 model.cpp:58] SIGNAL: SELL @ 57983.5
W0413 16:06:48.761430 19807506 strategy.cpp:121] Trading *NOT* enabled
I0413 16:06:48.780788 19807506 model.cpp:50] DIRECTION: SELLING
I0413 16:06:48.793745 19807506 model.cpp:44] SIGNAL: BUY @ 57967.5
W0413 16:06:48.793802 19807506 strategy.cpp:121] Trading *NOT* enabled
I0413 16:06:48.829393 19807506 model.cpp:64] DIRECTION: BUYING
I0413 16:06:48.833737 19807506 model.cpp:58] SIGNAL: SELL @ 57954.5
W0413 16:06:48.833755 19807506 strategy.cpp:121] Trading *NOT* enabled
...
I0413 16:06:50.325360 19807506 model.cpp:64] DIRECTION: BUYING
I0413 16:06:50.325364 19807506 model.cpp:58] SIGNAL: SELL @ 57942
W0413 16:06:50.325368 19807506 strategy.cpp:121] Trading *NOT* enabled
I0413 16:06:50.370552 19807506 model.cpp:64] DIRECTION: BUYING
I0413 16:06:50.370575 19807506 model.cpp:58] SIGNAL: SELL @ 58002
W0413 16:06:50.370579 19807506 strategy.cpp:121] Trading *NOT* enabled
I0413 16:06:50.370583 19807506 model.cpp:64] DIRECTION: BUYING
I0413 16:06:50.370587 19807506 model.cpp:58] SIGNAL: SELL @ 58002
W0413 16:06:50.370589 19807506 strategy.cpp:121] Trading *NOT* enabled
I0413 16:06:50.376169 19807506 instrument.cpp:54] [deribit:BTC-PERPETUAL] connected=false
I0413 16:06:50.584351 19807506 service.cpp:54] ===== STOP =====
```

### Order Management

Placement of limit orders is controlled by the `--enable-trading` flag

```bash
./roq-samples-example-3 \
    --name "trader" \
    --simulation \
    --enable_trading \
    $CONDA_PREFIX/share/roq/data/deribit.roq
```

```text
I0414 05:51:27.424289 117463 service.cpp:50] ===== START =====
I0414 05:51:27.431203 117463 instrument.cpp:47] [deribit:BTC-PERPETUAL] connected=true
I0414 05:51:27.432257 117463 instrument.cpp:109] [deribit:BTC-PERPETUAL] order_management=true
I0414 05:51:27.432653 117463 instrument.cpp:144] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0414 05:51:27.447151 117463 instrument.cpp:127] [deribit:BTC-PERPETUAL] tick_size=0.5
I0414 05:51:27.447168 117463 instrument.cpp:130] [deribit:BTC-PERPETUAL] min_trade_vol=1
I0414 05:51:27.447173 117463 instrument.cpp:133] [deribit:BTC-PERPETUAL] multiplier=10
I0414 05:51:27.447791 117463 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=true
I0414 05:51:27.447797 117463 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=true
I0414 05:51:27.448027 117463 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=false
I0414 05:51:27.448035 117463 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=false
I0414 05:51:27.448045 117463 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=true
I0414 05:51:27.448047 117463 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=true
I0414 05:51:27.600544 117463 model.cpp:50] DIRECTION: SELLING
I0414 05:51:27.603843 117463 model.cpp:64] DIRECTION: BUYING
I0414 05:51:27.607118 117463 model.cpp:50] DIRECTION: SELLING
I0414 05:51:27.618410 117463 model.cpp:64] DIRECTION: BUYING
I0414 05:51:27.625672 117463 model.cpp:58] SIGNAL: SELL @ 57983.5
I0414 05:51:27.625703 117463 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=1, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=1, external_account="", external_order_id="", request_id=""}
I0414 05:51:27.625717 117463 strategy.cpp:79] OrderUpdate={stream_id=0, account="A1", order_id=1, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=SELL, price=57984, remaining_quantity=1, traded_quantity=0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=1, external_account="", external_order_id=""}
I0414 05:51:27.625724 117463 instrument.cpp:212] [deribit:BTC-PERPETUAL] position=0
I0414 05:51:27.629310 117463 model.cpp:50] DIRECTION: SELLING
I0414 05:51:27.631775 117463 model.cpp:44] SIGNAL: BUY @ 57967.5
I0414 05:51:27.631780 117463 strategy.cpp:131] *** ANOTHER ORDER IS WORKING ***
I0414 05:51:27.631783 117463 strategy.cpp:133] *** CANCEL WORKING ORDER ***
I0414 05:51:27.631800 117463 strategy.cpp:79] OrderUpdate={stream_id=0, account="A1", order_id=1, exchange="deribit", symbol="BTC-PERPETUAL", status=CANCELED, side=SELL, price=57984, remaining_quantity=1, traded_quantity=0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=1, external_account="", external_order_id=""}
I0414 05:51:27.631804 117463 instrument.cpp:212] [deribit:BTC-PERPETUAL] position=0
I0414 05:51:27.631809 117463 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=1, type=CANCEL_ORDER, origin=GATEWAY, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=1, external_account="", external_order_id="", request_id=""}
I0414 05:51:27.637272 117463 model.cpp:64] DIRECTION: BUYING
I0414 05:51:27.638560 117463 model.cpp:58] SIGNAL: SELL @ 57954.5
I0414 05:51:27.638580 117463 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=2, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=2, external_account="", external_order_id="", request_id=""}
I0414 05:51:27.638589 117463 strategy.cpp:79] OrderUpdate={stream_id=0, account="A1", order_id=2, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=SELL, price=57955, remaining_quantity=1, traded_quantity=0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=2, external_account="", external_order_id=""}
I0414 05:51:27.638592 117463 instrument.cpp:212] [deribit:BTC-PERPETUAL] position=0
I0414 05:51:27.638894 117463 strategy.cpp:89] TradeUpdate={stream_id=0, account="A1", order_id=2, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=2, external_account="", external_order_id="", fills=[{quantity=1, price=57955, trade_id=1, gateway_trade_id=0, external_trade_id=""}]}
I0414 05:51:27.638904 117463 strategy.cpp:79] OrderUpdate={stream_id=0, account="A1", order_id=2, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=SELL, price=57955, remaining_quantity=0, traded_quantity=1, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=2, external_account="", external_order_id=""}
I0414 05:51:27.638908 117463 instrument.cpp:212] [deribit:BTC-PERPETUAL] position=-1
I0414 05:51:27.639454 117463 model.cpp:64] DIRECTION: BUYING
I0414 05:51:27.639459 117463 model.cpp:58] SIGNAL: SELL @ 57963.5
I0414 05:51:27.639461 117463 strategy.cpp:144] *** CAN'T INCREASE POSITION ***
I0414 05:51:27.644194 117463 model.cpp:50] DIRECTION: SELLING
I0414 05:51:27.646197 117463 model.cpp:44] SIGNAL: BUY @ 57957.5
...
```

### Live Trading

Switching to live trading

```bash
./roq-samples-example-3 \
    --name "trader" \
    ~/deribit.sock
```

Then add the `--enable-trading` flag if you really want orders to be placed on
the market.
