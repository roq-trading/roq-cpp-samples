# Example 1

Demonstrates very basic event processing.

* Define the regex patterns used for subscribing market data.
* Use the Trader interface to automatically asynchronously manage connections
  and dispatch events.
* Use the asynchronous logger to print relevant incoming events.


## Prerequisites

Please refer [here](../../../../README.md#gateways-install-configure-run) to
install, configure and run the required market gateways.


## Using

The example requires the `name` option specifying the login name used
when accessing the market gateway(s).

Further arguments are UNIX domain socket end-points for the market gateways.

### Single Gateway

```bash
./roq-samples-example-1 \
    --name "example-1" \
    ~/deribit.sock
```

```text
I0724 15:51:46.524259 5300 application.cpp:42] ===== START =====
I0724 15:51:46.525527 5300 service.cpp:39] The metrics service will *not* be started
I0724 15:51:46.532732 5300 controller.cpp:108] Dispatching...
I0724 15:51:46.532791 5300 controller.cpp:112] Starting event loop thread...
I0724 15:51:46.533343 5301 controller.cpp:148] Event loop thread is now running
I0724 15:51:47.533882 5301 session_manager.cpp:44] Connecting "unix:///home/thraneh/deribit.sock"
I0724 15:51:47.534042 5301 session.cpp:38] Adding name="deribit" (user_id=3)
I0724 15:51:47.534086 5300 pollster.cpp:403] Adding name="deribit" (user_id=3)
I0724 15:51:47.534107 5300 main.cpp:90] [0:deribit] Connection={status=CONNECTED}
I0724 15:51:47.534302 5300 main.cpp:97] [0:deribit] DownloadBegin={account=""}
I0724 15:51:47.534308 5300 main.cpp:111] [0:deribit] MarketDataStatus={status=READY}
I0724 15:51:47.534384 5300 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-25DEC20", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 15:51:47.534392 5300 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-26MAR21", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 15:51:47.534402 5300 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-25SEP20", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 15:51:47.534509 5300 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-26MAR21", trading_status=OPEN}
I0724 15:51:47.534514 5300 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-25DEC20", trading_status=OPEN}
I0724 15:51:47.534582 5300 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-25SEP20", trading_status=OPEN}
I0724 15:51:47.534726 5300 main.cpp:104] [0:deribit] DownloadEnd={account="", max_order_id=0}
...
```

Noteworthy observations

* Download is automatic and will happen immediately after the connection has
  been established.
* Reference data may include important trade parameters such as tick-size,
  multiplier, minimum trade size, etc.
* Market status (open, closed, halt, etc)

### Verbose Logging

```bash
ROQ_v=1 ./roq-samples-example-1 \
    --name "example-1" \
    ~/deribit.sock
```

```text
I0724 16:06:25.011642 14645 application.cpp:42] ===== START =====
I0724 16:06:25.013208 14645 service.cpp:39] The metrics service will *not* be started
I0724 16:06:25.020504 14645 controller.cpp:108] Dispatching...
I0724 16:06:25.020557 14645 controller.cpp:112] Starting event loop thread...
I0724 16:06:25.021013 14648 controller.cpp:148] Event loop thread is now running
I0724 16:06:26.021083 14648 session_manager.cpp:44] Connecting "unix:///home/thraneh/deribit.sock"
I0724 16:06:26.021227 14648 session.cpp:38] Adding name="deribit" (user_id=3)
I0724 16:06:26.021277 14645 pollster.cpp:366] Received context update
I0724 16:06:26.021286 14645 pollster.cpp:403] Adding name="deribit" (user_id=3)
I0724 16:06:26.021297 14645 main.cpp:90] [0:deribit] Connection={status=CONNECTED}
I0724 16:06:26.021545 14645 bridge.cpp:50] DownloadBeginEvent={message_info={source=0, source_name="deribit", source_session_id="f3a2af4182d546108b1df2291e5526dc", source_seqno=170307, receive_time_utc=1595599586021532726ns, receive_time=156506253374101ns, source_send_time=156506253373589ns, source_receive_time=156506253373589ns, origin_create_time=156506253373589ns, origin_create_time_utc=0ns, is_last=true, opaque=18374689778206507008}, download_begin={account=""}}
I0724 16:06:26.021552 14645 main.cpp:97] [0:deribit] DownloadBegin={account=""}
I0724 16:06:26.021560 14645 bridge.cpp:78] MarketDataStatusEvent={message_info={source=0, source_name="deribit", source_session_id="f3a2af4182d546108b1df2291e5526dc", source_seqno=170308, receive_time_utc=1595599586021555720ns, receive_time=156506253397110ns, source_send_time=156506253374509ns, source_receive_time=156506253374509ns, origin_create_time=156506253374509ns, origin_create_time_utc=0ns, is_last=true, opaque=18374689778206507008}, market_data_status={status=READY}}
I0724 16:06:26.021568 14645 main.cpp:111] [0:deribit] MarketDataStatus={status=READY}
I0724 16:06:26.021635 14645 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-25DEC20", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:06:26.021641 14645 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-26MAR21", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:06:26.021655 14645 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-25SEP20", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:06:26.021783 14645 bridge.cpp:120] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="f3a2af4182d546108b1df2291e5526dc", source_seqno=170312, receive_time_utc=1595599586021779851ns, receive_time=156506253621226ns, source_send_time=156506253620891ns, source_receive_time=156506253620891ns, origin_create_time=156506253620891ns, origin_create_time_utc=0ns, is_last=true, opaque=18375252728159928445}, market_status={exchange="deribit", symbol="BTC-26MAR21", trading_status=OPEN}}
I0724 16:06:26.021789 14645 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-26MAR21", trading_status=OPEN}
I0724 16:06:26.021798 14645 bridge.cpp:120] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="f3a2af4182d546108b1df2291e5526dc", source_seqno=170313, receive_time_utc=1595599586021794339ns, receive_time=156506253635720ns, source_send_time=156506253622398ns, source_receive_time=156506253622398ns, origin_create_time=156506253622398ns, origin_create_time_utc=0ns, is_last=true, opaque=18375252728159928321}, market_status={exchange="deribit", symbol="BTC-25DEC20", trading_status=OPEN}}
I0724 16:06:26.021803 14645 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-25DEC20", trading_status=OPEN}
I0724 16:06:26.021878 14645 bridge.cpp:120] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="f3a2af4182d546108b1df2291e5526dc", source_seqno=170314, receive_time_utc=1595599586021875360ns, receive_time=156506253716735ns, source_send_time=156506253716422ns, source_receive_time=156506253716422ns, origin_create_time=156506253716422ns, origin_create_time_utc=0ns, is_last=true, opaque=18375252728159928382}, market_status={exchange="deribit", symbol="BTC-25SEP20", trading_status=OPEN}}
I0724 16:06:26.021883 14645 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-25SEP20", trading_status=OPEN}
I0724 16:06:26.021989 14645 main.cpp:140] [0:deribit] MarketByPriceUpdate={exchange="deribit", symbol="BTC-25SEP20", bids=[{price=9653.0, quantity=17201.0}, {price=9650.5, quantity=63.0}, {price=9650.0, quantity=1048.0}, {price=9649.5, quantity=450.0}, {price=9649.0, quantity=1397.0}, {price=9648.5, quantity=3000.0}, {price=9648.0, quantity=2450.0}, {price=9647.5, quantity=1800.0}, {price=9647.0, quantity=2687.0}, {price=9646.5, quantity=1900.0}, {price=9640.0, quantity=50.0}, {price=9630.0, quantity=103.0}, {price=9627.0, quantity=26.0}, {price=9620.0, quantity=100.0}, {price=9600.0, quantity=5.0}, {price=9554.5, quantity=955.0}, {price=9554.0, quantity=955.0}, {price=9553.0, quantity=1910.0}, {price=9551.5, quantity=1910.0}, {price=9544.0, quantity=954.0}, {price=9542.0, quantity=954.0}, {price=9541.0, quantity=954.0}, {price=9540.5, quantity=1908.0}, {price=9539.0, quantity=3814.0}, {price=9538.0, quantity=953.0}, {price=9537.0, quantity=1907.0}, {price=9535.5, quantity=953.0}, {price=9534.0, quantity=1906.0}, {price=9526.0, quantity=952.0}, {price=9524.0, quantity=1904.0}, {price=9438.5, quantity=330.0}, {price=9164.0, quantity=7144.0}, {price=8900.0, quantity=50.0}, {price=8100.0, quantity=7000.0}, {price=8000.0, quantity=1.0}, {price=7900.0, quantity=50.0}, {price=7700.0, quantity=100000.0}, {price=7500.0, quantity=7000.0}, {price=7475.0, quantity=25000.0}, {price=7450.0, quantity=5000.0}, {price=7250.0, quantity=5000.0}, {price=7200.0, quantity=7000.0}, {price=6428.5, quantity=20.0}, {price=6276.0, quantity=728.0}, {price=5000.0, quantity=100000.0}, {price=2500.5, quantity=5.0}, {price=2500.0, quantity=1.0}], asks=[{price=9653.5, quantity=6100.0}, {price=9654.0, quantity=410.0}, {price=9654.5, quantity=150.0}, {price=9655.0, quantity=1.0}, {price=9655.5, quantity=170.0}, {price=9656.5, quantity=250.0}, {price=9657.0, quantity=500.0}, {price=9658.5, quantity=199.0}, {price=9659.0, quantity=1.0}, {price=9659.5, quantity=328.0}, {price=9660.0, quantity=102.0}, {price=9670.0, quantity=100.0}, {price=9680.0, quantity=100.0}, {price=9690.0, quantity=103.0}, {price=9695.0, quantity=15.0}, {price=9700.0, quantity=100.0}, {price=9710.0, quantity=100.0}, {price=9720.0, quantity=100.0}, {price=9730.0, quantity=100.0}, {price=9740.0, quantity=100.0}, {price=9750.0, quantity=110.0}, {price=9760.0, quantity=100.0}, {price=9770.0, quantity=100.0}, {price=9780.0, quantity=100.0}, {price=9788.5, quantity=20.0}, {price=9790.0, quantity=100.0}, {price=9800.0, quantity=100.0}, {price=9807.5, quantity=980.0}, {price=9809.5, quantity=1961.0}, {price=9810.0, quantity=100.0}, {price=9818.0, quantity=981.0}, {price=9820.0, quantity=1082.0}, {price=9820.5, quantity=1964.0}, {price=9823.0, quantity=1964.0}, {price=9823.5, quantity=982.0}, {price=9825.5, quantity=1965.0}, {price=9826.5, quantity=982.0}, {price=9827.0, quantity=982.0}, {price=9827.5, quantity=1965.0}, {price=9828.5, quantity=1965.0}, {price=9830.0, quantity=100.0}, {price=9838.0, quantity=1966.0}, {price=9840.0, quantity=3936.0}, {price=10200.0, quantity=150.0}, {price=10500.0, quantity=351.0}, {price=20000.0, quantity=1.0}, {price=25000.0, quantity=1.0}, {price=30000.0, quantity=1.0}, {price=39999.5, quantity=5.0}, {price=40000.0, quantity=1.0}], snapshot=true, exchange_time_utc=0ns}
I0724 16:06:26.022029 14645 main.cpp:140] [0:deribit] MarketByPriceUpdate={exchange="deribit", symbol="BTC-26MAR21", bids=[{price=9862.5, quantity=95.0}, {price=9862.0, quantity=10.0}, {price=9861.5, quantity=999.0}, {price=9858.0, quantity=110.0}, {price=9853.5, quantity=13.0}, {price=9853.0, quantity=2050.0}, {price=9850.0, quantity=21.0}, {price=9849.0, quantity=9.0}, {price=9847.0, quantity=100.0}, {price=9846.5, quantity=2700.0}, {price=9845.5, quantity=1155.0}, {price=9505.0, quantity=3.0}, {price=9500.0, quantity=500.0}, {price=9200.0, quantity=1000.0}, {price=9055.0, quantity=453.0}, {price=8815.0, quantity=100.0}, {price=8600.0, quantity=5000.0}, {price=8000.0, quantity=1000.0}, {price=7300.0, quantity=1000.0}, {price=7250.0, quantity=5000.0}, {price=1200.0, quantity=100.0}, {price=100.0, quantity=2.0}], asks=[{price=9863.0, quantity=360.0}, {price=9863.5, quantity=49.0}, {price=9864.5, quantity=50.0}, {price=9867.5, quantity=50.0}, {price=9870.0, quantity=12.0}, {price=9871.5, quantity=146.0}, {price=9872.0, quantity=2700.0}, {price=9873.5, quantity=9.0}, {price=9874.5, quantity=990.0}, {price=9880.0, quantity=659.0}, {price=10014.0, quantity=7960.0}, {price=10100.0, quantity=10000.0}, {price=10194.0, quantity=10000.0}, {price=10300.0, quantity=10000.0}, {price=10500.0, quantity=10000.0}, {price=10600.0, quantity=10000.0}, {price=10700.0, quantity=10000.0}, {price=10800.0, quantity=10000.0}, {price=10900.0, quantity=10000.0}, {price=11000.0, quantity=10000.0}, {price=11200.0, quantity=20000.0}, {price=11300.0, quantity=20000.0}, {price=11400.0, quantity=20000.0}, {price=11500.0, quantity=20000.0}, {price=11600.0, quantity=20000.0}, {price=11700.0, quantity=20000.0}, {price=11800.0, quantity=20000.0}, {price=11900.0, quantity=20000.0}, {price=12000.0, quantity=20000.0}, {price=14227.0, quantity=14227.0}, {price=15000.0, quantity=1000.0}], snapshot=true, exchange_time_utc=0ns}
I0724 16:06:26.022053 14645 main.cpp:140] [0:deribit] MarketByPriceUpdate={exchange="deribit", symbol="BTC-25DEC20", bids=[{price=9754.0, quantity=6551.0}, {price=9753.0, quantity=500.0}, {price=9752.5, quantity=517.0}, {price=9751.5, quantity=250.0}, {price=9750.5, quantity=66.0}, {price=9750.0, quantity=1499.0}, {price=9749.5, quantity=981.0}, {price=9749.0, quantity=991.0}, {price=9748.5, quantity=991.0}, {price=9748.0, quantity=2703.0}, {price=9515.5, quantity=2.0}, {price=9350.0, quantity=2.0}, {price=9250.0, quantity=1000.0}, {price=8897.0, quantity=500.0}, {price=8606.5, quantity=3333.0}, {price=8600.0, quantity=5000.0}, {price=8481.0, quantity=500.0}, {price=7200.0, quantity=7000.0}, {price=50.0, quantity=5.0}], asks=[{price=9756.0, quantity=142.0}, {price=9756.5, quantity=250.0}, {price=9759.0, quantity=12.0}, {price=9759.5, quantity=12.0}, {price=9760.0, quantity=52.0}, {price=9760.5, quantity=250.0}, {price=9761.0, quantity=38.0}, {price=9762.0, quantity=32.0}, {price=9762.5, quantity=322.0}, {price=9763.0, quantity=100.0}, {price=9764.5, quantity=5.0}], snapshot=true, exchange_time_utc=0ns}
I0724 16:06:26.022066 14645 bridge.cpp:64] DownloadEndEvent={message_info={source=0, source_name="deribit", source_session_id="f3a2af4182d546108b1df2291e5526dc", source_seqno=170321, receive_time_utc=1595599586022062056ns, receive_time=156506253903441ns, source_send_time=156506253890098ns, source_receive_time=156506253890098ns, origin_create_time=156506253890098ns, origin_create_time_utc=0ns, is_last=true, opaque=18374689778206507008}, download_end={account="", max_order_id=0}}
I0724 16:06:26.022071 14645 main.cpp:104] [0:deribit] DownloadEnd={account="", max_order_id=0}
I0724 16:06:26.253707 14645 main.cpp:140] [0:deribit] MarketByPriceUpdate={exchange="deribit", symbol="BTC-26MAR21", bids=[], asks=[{price=9863.0, quantity=0.0}, {price=9880.0, quantity=1328.0}], snapshot=false, exchange_time_utc=1595599586241000000ns}
I0724 16:06:26.255148 14645 main.cpp:140] [0:deribit] MarketByPriceUpdate={exchange="deribit", symbol="BTC-26MAR21", bids=[{price=9862.5, quantity=0.0}, {price=9844.0, quantity=690.0}], asks=[], snapshot=false, exchange_time_utc=1595599586244000000ns}
I0724 16:06:26.264210 14645 main.cpp:140] [0:deribit] MarketByPriceUpdate={exchange="deribit", symbol="BTC-25SEP20", bids=[], asks=[{price=9653.5, quantity=50.0}, {price=9662.5, quantity=1944.0}], snapshot=false, exchange_time_utc=1595599586252000000ns}
...
```

Noteworthy observations

* MarketByPrice (the order book) includes a full snapshot when received during
  the download phase.
* Live MarketByPrice updates (received after the download phase) will only
  include incremental changes.


### Multiple Connections

```bash
./roq-samples-example-1 \
    --name "example-1" \
    ~/deribit.sock \
    ~/coinbase-pro.sock
```

```text
I0724 16:14:38.174796 19897 application.cpp:42] ===== START =====
I0724 16:14:38.176622 19897 service.cpp:39] The metrics service will *not* be started
I0724 16:14:38.181753 19897 controller.cpp:108] Dispatching...
I0724 16:14:38.181802 19897 controller.cpp:112] Starting event loop thread...
I0724 16:14:38.182142 19898 controller.cpp:148] Event loop thread is now running
I0724 16:14:39.182242 19898 session_manager.cpp:44] Connecting "unix:///home/thraneh/coinbase-pro.sock"
I0724 16:14:39.182332 19898 session_manager.cpp:44] Connecting "unix:///home/thraneh/deribit.sock"
I0724 16:14:39.182434 19898 session.cpp:38] Adding name="coinbase-pro" (user_id=3)
I0724 16:14:39.182473 19897 pollster.cpp:403] Adding name="coinbase-pro" (user_id=3)
I0724 16:14:39.182492 19897 main.cpp:90] [1:coinbase-pro] Connection={status=CONNECTED}
I0724 16:14:39.182515 19898 session.cpp:38] Adding name="deribit" (user_id=3)
I0724 16:14:39.182586 19897 pollster.cpp:403] Adding name="deribit" (user_id=3)
I0724 16:14:39.182593 19897 main.cpp:90] [0:deribit] Connection={status=CONNECTED}
I0724 16:14:39.182791 19897 main.cpp:97] [1:coinbase-pro] DownloadBegin={account=""}
I0724 16:14:39.182797 19897 main.cpp:111] [1:coinbase-pro] MarketDataStatus={status=READY}
I0724 16:14:39.182810 19897 main.cpp:125] [1:coinbase-pro] ReferenceData={exchange="coinbase-pro", symbol="BTC-GBP", security_type=SPOT, currency="BTC", settlement_currency="GBP", commission_currency="BTC", tick_size=0.010000000000000002, limit_up=nan, limit_down=nan, multiplier=1.0, min_trade_vol=0.0010000000000000002, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:14:39.182822 19897 main.cpp:125] [1:coinbase-pro] ReferenceData={exchange="coinbase-pro", symbol="BTC-USD", security_type=SPOT, currency="BTC", settlement_currency="USD", commission_currency="BTC", tick_size=0.010000000000000002, limit_up=nan, limit_down=nan, multiplier=1.0, min_trade_vol=0.0010000000000000002, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:14:39.182831 19897 main.cpp:125] [1:coinbase-pro] ReferenceData={exchange="coinbase-pro", symbol="BTC-EUR", security_type=SPOT, currency="BTC", settlement_currency="EUR", commission_currency="BTC", tick_size=0.010000000000000002, limit_up=nan, limit_down=nan, multiplier=1.0, min_trade_vol=0.0010000000000000002, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:14:39.182842 19897 main.cpp:132] [1:coinbase-pro] MarketStatus={exchange="coinbase-pro", symbol="BTC-USD", trading_status=OPEN}
I0724 16:14:39.182848 19897 main.cpp:132] [1:coinbase-pro] MarketStatus={exchange="coinbase-pro", symbol="BTC-GBP", trading_status=OPEN}
I0724 16:14:39.182854 19897 main.cpp:132] [1:coinbase-pro] MarketStatus={exchange="coinbase-pro", symbol="BTC-EUR", trading_status=OPEN}
I0724 16:14:39.182871 19897 main.cpp:104] [1:coinbase-pro] DownloadEnd={account="", max_order_id=0}
I0724 16:14:39.183033 19897 main.cpp:97] [0:deribit] DownloadBegin={account=""}
I0724 16:14:39.183038 19897 main.cpp:111] [0:deribit] MarketDataStatus={status=READY}
I0724 16:14:39.183111 19897 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-25DEC20", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:14:39.183117 19897 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-26MAR21", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:14:39.183133 19897 main.cpp:125] [0:deribit] ReferenceData={exchange="deribit", symbol="BTC-25SEP20", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, limit_up=nan, limit_down=nan, multiplier=10.0, min_trade_vol=1.0, option_type=UNDEFINED, strike_currency="", strike_price=nan}
I0724 16:14:39.183238 19897 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-26MAR21", trading_status=OPEN}
I0724 16:14:39.183242 19897 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-25DEC20", trading_status=OPEN}
I0724 16:14:39.183311 19897 main.cpp:132] [0:deribit] MarketStatus={exchange="deribit", symbol="BTC-25SEP20", trading_status=OPEN}
I0724 16:14:39.183456 19897 main.cpp:104] [0:deribit] DownloadEnd={account="", max_order_id=0}
```

Noteworthy observations

* Download is per gateway and must be managed as such. In particular, it is
  possible that multiple downloads can simultaneously be in progress.
