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
    $CONDA_PREFIX/share/roq-data/deribit.roq
```

```text
I0724 17:14:29.087824 24623 application.cpp:42] ===== START =====
I0724 17:14:29.096955 24623 main.cpp:182] [deribit:BTC-PERPETUAL] connection_status=CONNECTED
I0724 17:14:29.098500 24623 main.cpp:232] [deribit:BTC-PERPETUAL] market_data_status=DOWNLOADING
I0724 17:14:29.098515 24623 main.cpp:246] [deribit:BTC-PERPETUAL] order_manager_status=DOWNLOADING
I0724 17:14:29.100352 24623 main.cpp:263] [deribit:BTC-PERPETUAL] tick_size=0.5
I0724 17:14:29.100361 24623 main.cpp:270] [deribit:BTC-PERPETUAL] min_trade_vol=1.0
I0724 17:14:29.100363 24623 main.cpp:277] [deribit:BTC-PERPETUAL] multiplier=10.0
I0724 17:14:29.101471 24623 main.cpp:232] [deribit:BTC-PERPETUAL] market_data_status=READY
I0724 17:14:29.101481 24623 main.cpp:246] [deribit:BTC-PERPETUAL] order_manager_status=READY
I0724 17:14:29.112516 24623 main.cpp:292] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0724 17:14:29.112533 24623 main.cpp:417] [deribit:BTC-PERPETUAL] ready=true
I0724 17:14:29.211853 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.231700 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.240049 24623 main.cpp:563] SIGNAL: SELL @ 9022.5
W0724 17:14:29.240059 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.256192 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.262624 24623 main.cpp:547] SIGNAL: BUY @ 9019.5
W0724 17:14:29.262633 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.265499 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.265663 24623 main.cpp:563] SIGNAL: SELL @ 9024.5
W0724 17:14:29.265667 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.276872 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.277061 24623 main.cpp:563] SIGNAL: SELL @ 9030.5
W0724 17:14:29.277065 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.277340 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.278165 24623 main.cpp:563] SIGNAL: SELL @ 9030.5
W0724 17:14:29.278171 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.278756 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.278903 24623 main.cpp:563] SIGNAL: SELL @ 9032.0
W0724 17:14:29.278907 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.287508 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.287685 24623 main.cpp:563] SIGNAL: SELL @ 9035.0
W0724 17:14:29.287689 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.299199 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.299335 24623 main.cpp:563] SIGNAL: SELL @ 9040.5
W0724 17:14:29.299339 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.348992 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.349260 24623 main.cpp:547] SIGNAL: BUY @ 9037.0
W0724 17:14:29.349264 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.359741 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.361614 24623 main.cpp:547] SIGNAL: BUY @ 9036.5
W0724 17:14:29.361619 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.377842 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.378028 24623 main.cpp:563] SIGNAL: SELL @ 9040.5
W0724 17:14:29.378032 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.379380 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.379543 24623 main.cpp:563] SIGNAL: SELL @ 9041.5
W0724 17:14:29.379547 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.383796 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.386325 24623 main.cpp:547] SIGNAL: BUY @ 9040.5
W0724 17:14:29.386330 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.412382 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.415534 24623 main.cpp:547] SIGNAL: BUY @ 9038.5
W0724 17:14:29.415542 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.426148 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.426363 24623 main.cpp:563] SIGNAL: SELL @ 9043.5
W0724 17:14:29.426366 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.435213 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.435562 24623 main.cpp:563] SIGNAL: SELL @ 9047.0
W0724 17:14:29.435566 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.443384 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.443704 24623 main.cpp:563] SIGNAL: SELL @ 9053.0
W0724 17:14:29.443708 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.459873 24623 main.cpp:555] DIRECTION: SELLING
I0724 17:14:29.461489 24623 main.cpp:547] SIGNAL: BUY @ 9053.0
W0724 17:14:29.461494 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.544011 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.544248 24623 main.cpp:563] SIGNAL: SELL @ 9048.5
W0724 17:14:29.544253 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.545456 24623 main.cpp:571] DIRECTION: BUYING
I0724 17:14:29.545805 24623 main.cpp:563] SIGNAL: SELL @ 9050.0
W0724 17:14:29.545808 24623 main.cpp:760] Trading *NOT* enabled
I0724 17:14:29.555737 24623 main.cpp:182] [deribit:BTC-PERPETUAL] connection_status=DISCONNECTED
I0724 17:14:29.555747 24623 main.cpp:417] [deribit:BTC-PERPETUAL] ready=false
I0724 17:14:29.568697 24623 application.cpp:47] ===== STOP =====
```

### Order Management

Placement of limit orders is controlled by the `--enable-trading` flag

```bash
./roq-samples-example-3 \
    --name "trader" \
    --simulation \
    --enable-trading \
    $CONDA_PREFIX/share/roq-data/deribit.roq
```

```text
I0724 17:15:31.488550 25327 application.cpp:42] ===== START =====
I0724 17:15:31.510960 25327 main.cpp:182] [deribit:BTC-PERPETUAL] connection_status=CONNECTED
I0724 17:15:31.512764 25327 main.cpp:232] [deribit:BTC-PERPETUAL] market_data_status=DOWNLOADING
I0724 17:15:31.512780 25327 main.cpp:246] [deribit:BTC-PERPETUAL] order_manager_status=DOWNLOADING
I0724 17:15:31.515712 25327 main.cpp:263] [deribit:BTC-PERPETUAL] tick_size=0.5
I0724 17:15:31.515725 25327 main.cpp:270] [deribit:BTC-PERPETUAL] min_trade_vol=1.0
I0724 17:15:31.515729 25327 main.cpp:277] [deribit:BTC-PERPETUAL] multiplier=10.0
I0724 17:15:31.517312 25327 main.cpp:232] [deribit:BTC-PERPETUAL] market_data_status=READY
I0724 17:15:31.517334 25327 main.cpp:246] [deribit:BTC-PERPETUAL] order_manager_status=READY
I0724 17:15:31.530174 25327 main.cpp:292] [deribit:BTC-PERPETUAL] trading_status=OPEN
I0724 17:15:31.530190 25327 main.cpp:417] [deribit:BTC-PERPETUAL] ready=true
I0724 17:15:31.628597 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.648608 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.656911 25327 main.cpp:563] SIGNAL: SELL @ 9022.5
I0724 17:15:31.656954 25327 main.cpp:699] OrderAck={account="A1", order_id=1, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=1, external_order_id="", request_id=""}
I0724 17:15:31.656964 25327 main.cpp:708] OrderUpdate={account="A1", order_id=1, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=SELL, price=9023.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=1, external_order_id=""}
I0724 17:15:31.656973 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.666163 25327 main.cpp:719] TradeUpdate={account="A1", order_id=1, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=1, external_order_id="", fills=[{quantity=1.0, price=9023.0, trade_id=1, gateway_trade_id=0, external_trade_id=""}]}
I0724 17:15:31.666179 25327 main.cpp:708] OrderUpdate={account="A1", order_id=1, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=SELL, price=9023.0, remaining_quantity=0.0, traded_quantity=1.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=1, external_order_id=""}
I0724 17:15:31.666185 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.673261 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.679735 25327 main.cpp:547] SIGNAL: BUY @ 9019.5
I0724 17:15:31.679758 25327 main.cpp:699] OrderAck={account="A1", order_id=2, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=2, external_order_id="", request_id=""}
I0724 17:15:31.679766 25327 main.cpp:708] OrderUpdate={account="A1", order_id=2, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9019.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=2, external_order_id=""}
I0724 17:15:31.679770 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.682566 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.682753 25327 main.cpp:563] SIGNAL: SELL @ 9024.5
I0724 17:15:31.682759 25327 main.cpp:770] *** ANOTHER ORDER IS WORKING ***
I0724 17:15:31.682761 25327 main.cpp:772] *** CANCEL WORKING ORDER ***
I0724 17:15:31.682773 25327 main.cpp:708] OrderUpdate={account="A1", order_id=2, exchange="deribit", symbol="BTC-PERPETUAL", status=CANCELED, side=BUY, price=9019.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=2, external_order_id=""}
I0724 17:15:31.682777 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.682781 25327 main.cpp:699] OrderAck={account="A1", order_id=2, type=CANCEL_ORDER, origin=GATEWAY, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=2, external_order_id="", request_id=""}
I0724 17:15:31.694261 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.694475 25327 main.cpp:563] SIGNAL: SELL @ 9030.5
I0724 17:15:31.694481 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.694774 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.695586 25327 main.cpp:563] SIGNAL: SELL @ 9030.5
I0724 17:15:31.695591 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.696208 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.696375 25327 main.cpp:563] SIGNAL: SELL @ 9032.0
I0724 17:15:31.696379 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.704982 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.705178 25327 main.cpp:563] SIGNAL: SELL @ 9035.0
I0724 17:15:31.705183 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.716591 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.716748 25327 main.cpp:563] SIGNAL: SELL @ 9040.5
I0724 17:15:31.716753 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.766485 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.766783 25327 main.cpp:547] SIGNAL: BUY @ 9037.0
I0724 17:15:31.766804 25327 main.cpp:699] OrderAck={account="A1", order_id=3, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=3, external_order_id="", request_id=""}
I0724 17:15:31.766812 25327 main.cpp:708] OrderUpdate={account="A1", order_id=3, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9036.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=3, external_order_id=""}
I0724 17:15:31.766816 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.779422 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.781544 25327 main.cpp:547] SIGNAL: BUY @ 9036.5
I0724 17:15:31.781552 25327 main.cpp:770] *** ANOTHER ORDER IS WORKING ***
I0724 17:15:31.797947 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.798174 25327 main.cpp:563] SIGNAL: SELL @ 9040.5
I0724 17:15:31.798181 25327 main.cpp:770] *** ANOTHER ORDER IS WORKING ***
I0724 17:15:31.798184 25327 main.cpp:772] *** CANCEL WORKING ORDER ***
I0724 17:15:31.798211 25327 main.cpp:708] OrderUpdate={account="A1", order_id=3, exchange="deribit", symbol="BTC-PERPETUAL", status=CANCELED, side=BUY, price=9036.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=3, external_order_id=""}
I0724 17:15:31.798218 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.798226 25327 main.cpp:699] OrderAck={account="A1", order_id=3, type=CANCEL_ORDER, origin=GATEWAY, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=3, external_order_id="", request_id=""}
I0724 17:15:31.799573 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.799764 25327 main.cpp:563] SIGNAL: SELL @ 9041.5
I0724 17:15:31.799769 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.803970 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.806645 25327 main.cpp:547] SIGNAL: BUY @ 9040.5
I0724 17:15:31.806676 25327 main.cpp:699] OrderAck={account="A1", order_id=4, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=4, external_order_id="", request_id=""}
I0724 17:15:31.806687 25327 main.cpp:708] OrderUpdate={account="A1", order_id=4, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9040.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=4, external_order_id=""}
I0724 17:15:31.806695 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.832572 25327 main.cpp:719] TradeUpdate={account="A1", order_id=4, exchange="deribit", symbol="BTC-PERPETUAL", side=BUY, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=4, external_order_id="", fills=[{quantity=1.0, price=9040.0, trade_id=2, gateway_trade_id=0, external_trade_id=""}]}
I0724 17:15:31.832592 25327 main.cpp:708] OrderUpdate={account="A1", order_id=4, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=BUY, price=9040.0, remaining_quantity=0.0, traded_quantity=1.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=4, external_order_id=""}
I0724 17:15:31.832596 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.832879 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.835894 25327 main.cpp:547] SIGNAL: BUY @ 9038.5
I0724 17:15:31.835911 25327 main.cpp:699] OrderAck={account="A1", order_id=5, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=5, external_order_id="", request_id=""}
I0724 17:15:31.835917 25327 main.cpp:708] OrderUpdate={account="A1", order_id=5, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9038.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=5, external_order_id=""}
I0724 17:15:31.835921 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.846429 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.846640 25327 main.cpp:563] SIGNAL: SELL @ 9043.5
I0724 17:15:31.846644 25327 main.cpp:770] *** ANOTHER ORDER IS WORKING ***
I0724 17:15:31.846646 25327 main.cpp:772] *** CANCEL WORKING ORDER ***
I0724 17:15:31.846661 25327 main.cpp:708] OrderUpdate={account="A1", order_id=5, exchange="deribit", symbol="BTC-PERPETUAL", status=CANCELED, side=BUY, price=9038.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=5, external_order_id=""}
I0724 17:15:31.846665 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.846669 25327 main.cpp:699] OrderAck={account="A1", order_id=5, type=CANCEL_ORDER, origin=GATEWAY, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=5, external_order_id="", request_id=""}
I0724 17:15:31.855850 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.856217 25327 main.cpp:563] SIGNAL: SELL @ 9047.0
I0724 17:15:31.856237 25327 main.cpp:699] OrderAck={account="A1", order_id=6, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=6, external_order_id="", request_id=""}
I0724 17:15:31.856245 25327 main.cpp:708] OrderUpdate={account="A1", order_id=6, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=SELL, price=9047.5, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=6, external_order_id=""}
I0724 17:15:31.856248 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.856656 25327 main.cpp:719] TradeUpdate={account="A1", order_id=6, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=6, external_order_id="", fills=[{quantity=1.0, price=9047.5, trade_id=3, gateway_trade_id=0, external_trade_id=""}]}
I0724 17:15:31.856664 25327 main.cpp:708] OrderUpdate={account="A1", order_id=6, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=SELL, price=9047.5, remaining_quantity=0.0, traded_quantity=1.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=6, external_order_id=""}
I0724 17:15:31.856667 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.863929 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.864262 25327 main.cpp:563] SIGNAL: SELL @ 9053.0
I0724 17:15:31.864267 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.880486 25327 main.cpp:555] DIRECTION: SELLING
I0724 17:15:31.882124 25327 main.cpp:547] SIGNAL: BUY @ 9053.0
I0724 17:15:31.882149 25327 main.cpp:699] OrderAck={account="A1", order_id=7, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=7, external_order_id="", request_id=""}
I0724 17:15:31.882157 25327 main.cpp:708] OrderUpdate={account="A1", order_id=7, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=BUY, price=9052.5, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=7, external_order_id=""}
I0724 17:15:31.882161 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.905964 25327 main.cpp:719] TradeUpdate={account="A1", order_id=7, exchange="deribit", symbol="BTC-PERPETUAL", side=BUY, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=7, external_order_id="", fills=[{quantity=1.0, price=9052.5, trade_id=4, gateway_trade_id=0, external_trade_id=""}]}
I0724 17:15:31.905983 25327 main.cpp:708] OrderUpdate={account="A1", order_id=7, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=BUY, price=9052.5, remaining_quantity=0.0, traded_quantity=1.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=7, external_order_id=""}
I0724 17:15:31.905986 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.964389 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.964617 25327 main.cpp:563] SIGNAL: SELL @ 9048.5
I0724 17:15:31.964636 25327 main.cpp:699] OrderAck={account="A1", order_id=8, type=CREATE_ORDER, origin=EXCHANGE, status=ACCEPTED, error=UNDEFINED, text="", gateway_order_id=8, external_order_id="", request_id=""}
I0724 17:15:31.964645 25327 main.cpp:708] OrderUpdate={account="A1", order_id=8, exchange="deribit", symbol="BTC-PERPETUAL", status=WORKING, side=SELL, price=9049.0, remaining_quantity=1.0, traded_quantity=0.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=8, external_order_id=""}
I0724 17:15:31.964648 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=0.0
I0724 17:15:31.964931 25327 main.cpp:719] TradeUpdate={account="A1", order_id=8, exchange="deribit", symbol="BTC-PERPETUAL", side=SELL, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=8, external_order_id="", fills=[{quantity=1.0, price=9049.0, trade_id=5, gateway_trade_id=0, external_trade_id=""}]}
I0724 17:15:31.964938 25327 main.cpp:708] OrderUpdate={account="A1", order_id=8, exchange="deribit", symbol="BTC-PERPETUAL", status=COMPLETED, side=SELL, price=9049.0, remaining_quantity=0.0, traded_quantity=1.0, position_effect=UNDEFINED, order_template="", create_time_utc=0ns, update_time_utc=0ns, gateway_order_id=8, external_order_id=""}
I0724 17:15:31.964941 25327 main.cpp:370] [deribit:BTC-PERPETUAL] position=-1.0
I0724 17:15:31.965859 25327 main.cpp:571] DIRECTION: BUYING
I0724 17:15:31.966209 25327 main.cpp:563] SIGNAL: SELL @ 9050.0
I0724 17:15:31.966212 25327 main.cpp:783] *** CAN'T INCREASE POSITION ***
I0724 17:15:31.976401 25327 main.cpp:182] [deribit:BTC-PERPETUAL] connection_status=DISCONNECTED
I0724 17:15:31.976413 25327 main.cpp:417] [deribit:BTC-PERPETUAL] ready=false
I0724 17:15:31.989237 25327 application.cpp:47] ===== STOP =====
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
