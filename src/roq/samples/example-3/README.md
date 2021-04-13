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
    $CONDA_PREFIX/share/roq-data/deribit.roq
```

```text
I0413 16:09:56.277700 19810694 service.cpp:50] ===== START =====
I0413 16:09:56.355267 19810694 instrument.cpp:47] [deribit:BTC-PERPETUAL] connected=true
I0413 16:09:56.357376 19810694 instrument.cpp:109] [deribit:BTC-PERPETUAL] order_management=true
I0413 16:09:56.358901 19810694 instrument.cpp:144] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0413 16:09:56.418726 19810694 instrument.cpp:127] [deribit:BTC-PERPETUAL] tick_size=0.5
I0413 16:09:56.418743 19810694 instrument.cpp:130] [deribit:BTC-PERPETUAL] min_trade_vol=1
I0413 16:09:56.418747 19810694 instrument.cpp:133] [deribit:BTC-PERPETUAL] multiplier=10
I0413 16:09:56.420908 19810694 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=true
I0413 16:09:56.420919 19810694 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=true
I0413 16:09:56.421704 19810694 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=false
I0413 16:09:56.421736 19810694 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=false
I0413 16:09:56.421795 19810694 instrument.cpp:92] [deribit:BTC-PERPETUAL] market_data=true
I0413 16:09:56.421812 19810694 instrument.cpp:250] [deribit:BTC-PERPETUAL] ready=true
I0413 16:09:57.172749 19810694 model.cpp:50] DIRECTION: SELLING
I0413 16:09:57.189732 19810694 model.cpp:64] DIRECTION: BUYING
I0413 16:09:57.207867 19810694 model.cpp:50] DIRECTION: SELLING
I0413 16:09:57.265390 19810694 model.cpp:64] DIRECTION: BUYING
I0413 16:09:57.302656 19810694 model.cpp:58] SIGNAL: SELL @ 57983.5
I0413 16:09:57.303977 19810694 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=1, type=CREATE_ORDER, origin=GATEWAY, status=REJECTED, error=INVALID_ACCOUNT, text="", gateway_order_id=0, external_account="", external_order_id="", request_id=""}
I0413 16:09:57.321615 19810694 model.cpp:50] DIRECTION: SELLING
I0413 16:09:57.337898 19810694 model.cpp:44] SIGNAL: BUY @ 57967.5
I0413 16:09:57.337915 19810694 strategy.cpp:131] *** ANOTHER ORDER IS WORKING ***
I0413 16:09:57.337919 19810694 strategy.cpp:133] *** CANCEL WORKING ORDER ***
I0413 16:09:57.338379 19810694 strategy.cpp:71] OrderAck={stream_id=0, account="A1", order_id=1, type=CANCEL_ORDER, origin=GATEWAY, status=REJECTED, error=MODIFY_ORDER_NOT_SUPPORTED, text="", gateway_order_id=0, external_account="", external_order_id="", request_id=""}
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
