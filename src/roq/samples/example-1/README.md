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
    --name "trader" \
    ~/deribit.sock
```

```text
I0413 16:01:02.530790 19792386 service.cpp:50] ===== START =====
I0413 16:01:02.531982 19792386 service.cpp:34] The metrics service will *not* be started
I0413 16:01:02.552006 19792386 controller.cpp:74] session_id="6ca9cc67-61d0-4671-ab7e-db7284acb55e"
I0413 16:01:02.552400 19792386 controller.cpp:78] Dispatching...
I0413 16:01:02.553257 19792386 controller.cpp:82] Starting event loop thread...
I0413 16:01:02.554198 19792429 controller.cpp:113] Event loop thread is now running
I0413 16:01:03.553985 19792429 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-deribit/src/roq/deribit/deribit-test.sock"
I0413 16:01:03.554458 19792429 session.cpp:32] Adding name="deribit" (user_id=1)
I0413 16:01:03.554544 19792386 pollster.cpp:305] Adding name="deribit" (user_id=1)
I0413 16:01:03.554608 19792386 strategy.cpp:17] [0:deribit] connected={}
I0413 16:01:03.554865 19792386 strategy.cpp:49] [0:deribit] Settings={mbp_max_depth=0, mbp_allow_price_inversion=false, mbp_allow_fractional_tick_size=true}
I0413 16:01:03.554876 19792386 strategy.cpp:33] [0:deribit] DownloadBegin={account=""}
I0413 16:01:03.554892 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=1, account="A1", supports=0x3f0000, status=READY, type=FIX, priority=PRIMARY}
I0413 16:01:03.554900 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=2, account="A1", supports=0x10400000, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:01:03.554906 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=3, account="", supports=0x6, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:01:03.554912 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=4, account="", supports=0x69, status=READY, type=FIX, priority=PRIMARY}
I0413 16:01:03.554919 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=5, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:01:03.554991 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=6, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 16:01:03.554998 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=7, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 16:01:03.555005 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=8, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 16:01:03.555013 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=9, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:01:03.555020 19792386 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=10, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:01:03.555087 19792386 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-24SEP21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18621[86400]s, settlement_date=0[86400]s, expiry_datetime=1632470400s, expiry_datetime_utc=1632470400s}
I0413 16:01:03.555371 19792386 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-31DEC21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18669[86400]s, settlement_date=0[86400]s, expiry_datetime=1640937600s, expiry_datetime_utc=1640937600s}
I0413 16:01:03.555486 19792386 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-23APR21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18719[86400]s, settlement_date=0[86400]s, expiry_datetime=1619164800s, expiry_datetime_utc=1619164800s}
I0413 16:01:03.557827 19792386 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-25JUN21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18620[86400]s, settlement_date=0[86400]s, expiry_datetime=1624608000s, expiry_datetime_utc=1624608000s}
I0413 16:01:03.558991 19792386 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=5, exchange="deribit", symbol="BTC-25JUN21", trading_status=OPEN}
I0413 16:01:03.559545 19792386 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-31DEC21", trading_status=OPEN}
I0413 16:01:03.559754 19792386 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-23APR21", trading_status=OPEN}
I0413 16:01:03.559841 19792386 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-24SEP21", trading_status=OPEN}
I0413 16:01:03.564343 19792386 strategy.cpp:41] [0:deribit] DownloadEnd={account="", max_order_id=0}
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
    --name "trader" \
    ~/deribit.sock
```

```text
I0413 16:03:06.515752 19794421 service.cpp:50] ===== START =====
I0413 16:03:06.516739 19794421 service.cpp:34] The metrics service will *not* be started
I0413 16:03:06.527254 19794421 controller.cpp:74] session_id="cc0c1acb-fb54-4b2d-ac7e-f01e6b1f76f1"
I0413 16:03:06.527273 19794421 controller.cpp:78] Dispatching...
I0413 16:03:06.527441 19794421 controller.cpp:82] Starting event loop thread...
I0413 16:03:06.528374 19794461 controller.cpp:113] Event loop thread is now running
I0413 16:03:07.529548 19794461 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-deribit/src/roq/deribit/deribit-test.sock"
I0413 16:03:07.530166 19794461 session.cpp:32] Adding name="deribit" (user_id=1)
I0413 16:03:07.530252 19794421 pollster.cpp:261] Received context update
I0413 16:03:07.530271 19794421 pollster.cpp:305] Adding name="deribit" (user_id=1)
I0413 16:03:07.530340 19794421 strategy.cpp:17] [0:deribit] connected={}
I0413 16:03:07.530663 19794421 bridge.cpp:61] GatewaySettings={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36586, receive_time_utc=1618322587530617000ns, receive_time=3047806010072000ns, source_send_time=3047806010070000ns, source_receive_time=3047806010070000ns, origin_create_time=3047806010070000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, gateway_settings={mbp_max_depth=0, mbp_allow_price_inversion=false, mbp_allow_fractional_tick_size=true}}
I0413 16:03:07.530680 19794421 strategy.cpp:49] [0:deribit] Settings={mbp_max_depth=0, mbp_allow_price_inversion=false, mbp_allow_fractional_tick_size=true}
I0413 16:03:07.530705 19794421 bridge.cpp:41] DownloadBeginEvent={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36587, receive_time_utc=1618322587530686000ns, receive_time=3047806010141000ns, source_send_time=3047806010073000ns, source_receive_time=3047806010073000ns, origin_create_time=3047806010073000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, download_begin={account=""}}
I0413 16:03:07.530803 19794421 strategy.cpp:33] [0:deribit] DownloadBegin={account=""}
I0413 16:03:07.530841 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36588, receive_time_utc=1618322587530807000ns, receive_time=3047806010262000ns, source_send_time=3047806010078000ns, source_receive_time=3047806010078000ns, origin_create_time=3047806010078000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=1, account="A1", supports=0x3f0000, status=READY, type=FIX, priority=PRIMARY}}
I0413 16:03:07.530858 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=1, account="A1", supports=0x3f0000, status=READY, type=FIX, priority=PRIMARY}
I0413 16:03:07.530879 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36589, receive_time_utc=1618322587530861000ns, receive_time=3047806010316000ns, source_send_time=3047806010079000ns, source_receive_time=3047806010079000ns, origin_create_time=3047806010079000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=2, account="A1", supports=0x10400000, status=READY, type=WEB_SOCKET, priority=PRIMARY}}
I0413 16:03:07.530887 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=2, account="A1", supports=0x10400000, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:03:07.530907 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36590, receive_time_utc=1618322587530890000ns, receive_time=3047806010345000ns, source_send_time=3047806010080000ns, source_receive_time=3047806010080000ns, origin_create_time=3047806010080000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=3, account="", supports=0x6, status=READY, type=WEB_SOCKET, priority=PRIMARY}}
I0413 16:03:07.531262 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=3, account="", supports=0x6, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:03:07.531291 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36591, receive_time_utc=1618322587531269000ns, receive_time=3047806010723000ns, source_send_time=3047806010084000ns, source_receive_time=3047806010084000ns, origin_create_time=3047806010084000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=4, account="", supports=0x69, status=READY, type=FIX, priority=PRIMARY}}
I0413 16:03:07.531301 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=4, account="", supports=0x69, status=READY, type=FIX, priority=PRIMARY}
I0413 16:03:07.532016 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36592, receive_time_utc=1618322587531984000ns, receive_time=3047806011439000ns, source_send_time=3047806010087000ns, source_receive_time=3047806010087000ns, origin_create_time=3047806010087000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=5, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}}
I0413 16:03:07.532031 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=5, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:03:07.532051 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36593, receive_time_utc=1618322587532034000ns, receive_time=3047806011489000ns, source_send_time=3047806010090000ns, source_receive_time=3047806010090000ns, origin_create_time=3047806010090000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=6, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}}
I0413 16:03:07.532088 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=6, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 16:03:07.532110 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36594, receive_time_utc=1618322587532092000ns, receive_time=3047806011547000ns, source_send_time=3047806010093000ns, source_receive_time=3047806010093000ns, origin_create_time=3047806010093000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=7, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}}
I0413 16:03:07.532329 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=7, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 16:03:07.532357 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36595, receive_time_utc=1618322587532335000ns, receive_time=3047806011790000ns, source_send_time=3047806010096000ns, source_receive_time=3047806010096000ns, origin_create_time=3047806010096000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=8, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}}
I0413 16:03:07.532367 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=8, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 16:03:07.532389 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36596, receive_time_utc=1618322587532371000ns, receive_time=3047806011826000ns, source_send_time=3047806010099000ns, source_receive_time=3047806010099000ns, origin_create_time=3047806010099000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=9, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}}
I0413 16:03:07.532830 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=9, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:03:07.532874 19794421 bridge.cpp:71] StreamStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36597, receive_time_utc=1618322587532836000ns, receive_time=3047806012291000ns, source_send_time=3047806010102000ns, source_receive_time=3047806010102000ns, origin_create_time=3047806010102000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, stream_status={stream_id=10, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}}
I0413 16:03:07.533157 19794421 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=10, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 16:03:07.533194 19794421 bridge.cpp:91] GatewayStatus={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36598, receive_time_utc=1618322587533163000ns, receive_time=3047806012618000ns, source_send_time=3047806010106000ns, source_receive_time=3047806010106000ns, origin_create_time=3047806010106000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, gateway_status={account="", supported=0x6f, available=0x6f, unavailable=0x0}}
I0413 16:03:07.533211 19794421 strategy.cpp:75] [0:deribit] ExternalLatency={account="", supported=0x6f, available=0x6f, unavailable=0x0}
I0413 16:03:07.533277 19794421 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-24SEP21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18621[86400]s, settlement_date=0[86400]s, expiry_datetime=1632470400s, expiry_datetime_utc=1632470400s}
I0413 16:03:07.533477 19794421 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-31DEC21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18669[86400]s, settlement_date=0[86400]s, expiry_datetime=1640937600s, expiry_datetime_utc=1640937600s}
I0413 16:03:07.533499 19794421 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-23APR21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18719[86400]s, settlement_date=0[86400]s, expiry_datetime=1619164800s, expiry_datetime_utc=1619164800s}
I0413 16:03:07.533883 19794421 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-25JUN21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18620[86400]s, settlement_date=0[86400]s, expiry_datetime=1624608000s, expiry_datetime_utc=1624608000s}
I0413 16:03:07.535187 19794421 bridge.cpp:111] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36603, receive_time_utc=1618322587535153000ns, receive_time=3047806014608000ns, source_send_time=3047806014602000ns, source_receive_time=3047806014602000ns, origin_create_time=3047806014602000ns, origin_create_time_utc=0ns, is_last=true, opaque=18375250529136672922}, market_status={stream_id=5, exchange="deribit", symbol="BTC-25JUN21", trading_status=OPEN}}
I0413 16:03:07.535337 19794421 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=5, exchange="deribit", symbol="BTC-25JUN21", trading_status=OPEN}
I0413 16:03:07.535752 19794421 bridge.cpp:111] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36604, receive_time_utc=1618322587535732000ns, receive_time=3047806015187000ns, source_send_time=3047806015184000ns, source_receive_time=3047806015184000ns, origin_create_time=3047806015184000ns, origin_create_time_utc=0ns, is_last=true, opaque=18375250529136673416}, market_status={stream_id=3, exchange="deribit", symbol="BTC-31DEC21", trading_status=OPEN}}
I0413 16:03:07.535829 19794421 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-31DEC21", trading_status=OPEN}
I0413 16:03:07.535971 19794421 bridge.cpp:111] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36605, receive_time_utc=1618322587535952000ns, receive_time=3047806015407000ns, source_send_time=3047806015403000ns, source_receive_time=3047806015403000ns, origin_create_time=3047806015403000ns, origin_create_time_utc=0ns, is_last=true, opaque=18375250529136673291}, market_status={stream_id=3, exchange="deribit", symbol="BTC-23APR21", trading_status=OPEN}}
I0413 16:03:07.535980 19794421 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-23APR21", trading_status=OPEN}
I0413 16:03:07.536061 19794421 bridge.cpp:111] MarketStatusEvent={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36606, receive_time_utc=1618322587536045000ns, receive_time=3047806015499000ns, source_send_time=3047806015495000ns, source_receive_time=3047806015495000ns, origin_create_time=3047806015495000ns, origin_create_time_utc=0ns, is_last=true, opaque=18375250529136673507}, market_status={stream_id=3, exchange="deribit", symbol="BTC-24SEP21", trading_status=OPEN}}
I0413 16:03:07.536092 19794421 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-24SEP21", trading_status=OPEN}
I0413 16:03:07.539263 19794421 strategy.cpp:100] [0:deribit] MarketByPriceUpdate={stream_id=7, exchange="deribit", symbol="BTC-25JUN21", bids=[{price=69147.5, quantity=108}, {price=69140.5, quantity=600}, {price=69123, quantity=600}, {price=69121, quantity=300}, {price=69118, quantity=280}, {price=69117.5, quantity=569}, {price=69116, quantity=3010}, {price=69113.5, quantity=635}, {price=69113, quantity=1482}, {price=69106, quantity=13053}, {price=69024.5, quantity=690}, {price=68990.5, quantity=500}, {price=68869, quantity=550}, {price=68830.5, quantity=14454}, {price=68642, quantity=6864}, {price=68641.5, quantity=6864}, {price=68641, quantity=6864}, {price=61144, quantity=1}, {price=61009, quantity=500000}, {price=56789, quantity=500000}, {price=56358, quantity=359700}, {price=43158, quantity=55417}, {price=36836, quantity=10000}, {price=35914, quantity=10000}, {price=30914, quantity=10000}, {price=28400, quantity=1420}, {price=10000, quantity=10}, {price=1000, quantity=1}, {price=1, quantity=4}], asks=[{price=69156, quantity=1200}, {price=69163.5, quantity=195}, {price=69177, quantity=89}, {price=69177.5, quantity=647}, {price=69178.5, quantity=1090}, {price=69188, quantity=700}, {price=69188.5, quantity=300}, {price=69190, quantity=440}, {price=69190.5, quantity=700}, {price=69191, quantity=700}, {price=69201, quantity=6170}, {price=69201.5, quantity=6920}, {price=69202, quantity=6920}, {price=69259.5, quantity=693}, {price=85011.5, quantity=100}], snapshot=true, exchange_time_utc=0ns}
I0413 16:03:07.541358 19794421 strategy.cpp:100] [0:deribit] MarketByPriceUpdate={stream_id=4, exchange="deribit", symbol="BTC-31DEC21", bids=[{price=75445, quantity=4191}, {price=75278, quantity=16141}, {price=75276.5, quantity=45055}, {price=74985.5, quantity=7499}, {price=74985, quantity=7499}, {price=74984.5, quantity=7499}, {price=71733, quantity=720}, {price=65012, quantity=500000}, {price=49080, quantity=116}, {price=48922, quantity=20}, {price=48874, quantity=571}, {price=48870, quantity=20}, {price=48835.5, quantity=20}, {price=48620.5, quantity=1}], asks=[{price=76083, quantity=72}, {price=76083.5, quantity=1000}, {price=76093.5, quantity=680}, {price=76111, quantity=14649}, {price=76138.5, quantity=1800}, {price=76140.5, quantity=343}, {price=76169, quantity=598}, {price=76169.5, quantity=577}, {price=76171, quantity=299}, {price=76199.5, quantity=240}, {price=76284, quantity=7628}, {price=76284.5, quantity=7628}, {price=76285, quantity=7628}, {price=76443, quantity=764}, {price=77052.5, quantity=80}], snapshot=true, exchange_time_utc=0ns}
I0413 16:03:07.541563 19794421 strategy.cpp:100] [0:deribit] MarketByPriceUpdate={stream_id=6, exchange="deribit", symbol="BTC-23APR21", bids=[{price=63846.5, quantity=200}, {price=63845.5, quantity=500}, {price=63845, quantity=120}, {price=63835.5, quantity=638}, {price=63829, quantity=7979}, {price=63822.5, quantity=509}, {price=63820, quantity=184}, {price=63817, quantity=3000}, {price=63795, quantity=551}, {price=63793.5, quantity=17151}, {price=63792.5, quantity=368}, {price=60000, quantity=6000}, {price=56812.5, quantity=51841}, {price=50000, quantity=5000}], asks=[{price=63860, quantity=16911}, {price=63862, quantity=613}, {price=63878, quantity=64}, {price=63896.5, quantity=120}, {price=63898, quantity=29863}, {price=63919, quantity=7990}, {price=63920.5, quantity=425}, {price=63924.5, quantity=3000}, {price=63936.5, quantity=35393}, {price=63948, quantity=639}, {price=63949.5, quantity=89}, {price=64709.5, quantity=2}], snapshot=true, exchange_time_utc=0ns}
I0413 16:03:07.541699 19794421 strategy.cpp:100] [0:deribit] MarketByPriceUpdate={stream_id=4, exchange="deribit", symbol="BTC-24SEP21", bids=[{price=73229, quantity=210}, {price=73228.5, quantity=2}, {price=73226, quantity=15377}, {price=73210, quantity=84}, {price=73207, quantity=500}, {price=73206.5, quantity=600}, {price=73196, quantity=454}, {price=73195, quantity=460}, {price=73189.5, quantity=300}, {price=73186, quantity=700}, {price=73183.5, quantity=1110}, {price=73059.5, quantity=731}, {price=72651.5, quantity=7265}, {price=72651, quantity=7265}, {price=72650.5, quantity=7265}, {price=60010, quantity=286697}, {price=56789, quantity=500000}, {price=35219.5, quantity=10000}, {price=26821, quantity=98352}, {price=26808, quantity=100000}, {price=26796, quantity=100000}, {price=26786, quantity=100000}, {price=26785, quantity=100000}, {price=26775.5, quantity=100000}, {price=26091, quantity=10000}, {price=25339, quantity=1000}, {price=24050, quantity=100}, {price=22220, quantity=22}], asks=[{price=73240.5, quantity=400}, {price=73241, quantity=300}, {price=73253.5, quantity=300}, {price=73266, quantity=300}, {price=73278.5, quantity=1000}, {price=73279, quantity=568}, {price=73279.5, quantity=12140}, {price=73282, quantity=5000}, {price=73282.5, quantity=619}, {price=73283, quantity=700}, {price=73436, quantity=734}, {price=73588.5, quantity=7359}, {price=73589, quantity=7359}, {price=73589.5, quantity=7359}], snapshot=true, exchange_time_utc=0ns}
I0413 16:03:07.542002 19794421 bridge.cpp:51] DownloadEndEvent={message_info={source=0, source_name="deribit", source_session_id="ee5f0553-3e4c-4ddb-85cb-ecc3e495f7be", source_seqno=36615, receive_time_utc=1618322587541980000ns, receive_time=3047806021435000ns, source_send_time=3047806021434000ns, source_receive_time=3047806021434000ns, origin_create_time=3047806021434000ns, origin_create_time_utc=0ns, is_last=true, opaque=18374687579183251456}, download_end={account="", max_order_id=0}}
I0413 16:03:07.542014 19794421 strategy.cpp:41] [0:deribit] DownloadEnd={account="", max_order_id=0}
I0413 16:03:07.553070 19794421 strategy.cpp:100] [0:deribit] MarketByPriceUpdate={stream_id=7, exchange="deribit", symbol="BTC-25JUN21", bids=[{price=68869, quantity=0}, {price=68876.5, quantity=550}], asks=[], snapshot=false, exchange_time_utc=1618322587582000000ns}
I0413 16:03:08.062139 19794421 strategy.cpp:100] [0:deribit] MarketByPriceUpdate={stream_id=4, exchange="deribit", symbol="BTC-31DEC21", bids=[{price=75275.5, quantity=16141}, {price=75278, quantity=0}], asks=[], snapshot=false, exchange_time_utc=1618322588091000000ns}
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
    --name "trader" \
    ~/deribit.sock \
    ~/coinbase-pro.sock
```

```text
I0413 15:59:30.509024 19791278 service.cpp:50] ===== START =====
I0413 15:59:30.509946 19791278 service.cpp:34] The metrics service will *not* be started
I0413 15:59:30.520652 19791278 controller.cpp:74] session_id="f7822338-a0fd-42df-b98d-b785413bd09c"
I0413 15:59:30.520667 19791278 controller.cpp:78] Dispatching...
I0413 15:59:30.520705 19791278 controller.cpp:82] Starting event loop thread...
I0413 15:59:30.521676 19791324 controller.cpp:113] Event loop thread is now running
I0413 15:59:31.522048 19791324 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-coinbase-pro/src/roq/coinbase_pro/coinbase-pro-sandbox.sock"
I0413 15:59:31.522273 19791324 session_manager.cpp:50] Connecting "unix:///Users/thraneh/dev/roq-dev/roq-deribit/src/roq/deribit/deribit-test.sock"
I0413 15:59:31.524098 19791324 session.cpp:32] Adding name="deribit" (user_id=1)
I0413 15:59:31.524227 19791324 session.cpp:32] Adding name="coinbase-pro" (user_id=1)
I0413 15:59:31.524273 19791278 pollster.cpp:305] Adding name="deribit" (user_id=1)
I0413 15:59:31.524396 19791278 strategy.cpp:17] [0:deribit] connected={}
I0413 15:59:31.524425 19791278 pollster.cpp:305] Adding name="coinbase-pro" (user_id=1)
I0413 15:59:31.524468 19791278 strategy.cpp:17] [1:coinbase-pro] connected={}
I0413 15:59:31.525599 19791278 strategy.cpp:49] [1:coinbase-pro] Settings={mbp_max_depth=0, mbp_allow_price_inversion=false, mbp_allow_fractional_tick_size=false}
I0413 15:59:31.525799 19791278 strategy.cpp:33] [1:coinbase-pro] DownloadBegin={account=""}
I0413 15:59:31.525921 19791278 strategy.cpp:57] [1:coinbase-pro] StreamStatus={stream_id=1, account="", supports=0x10000003, status=READY, type=REST, priority=PRIMARY}
I0413 15:59:31.525958 19791278 strategy.cpp:57] [1:coinbase-pro] StreamStatus={stream_id=2, account="A1", supports=0x1d0000, status=READY, type=FIX, priority=PRIMARY}
I0413 15:59:31.526008 19791278 strategy.cpp:57] [1:coinbase-pro] StreamStatus={stream_id=3, account="A1", supports=0x380000, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.526046 19791278 strategy.cpp:57] [1:coinbase-pro] StreamStatus={stream_id=4, account="", supports=0x7f, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.526216 19791278 strategy.cpp:83] [1:coinbase-pro] ReferenceData={stream_id=1, exchange="coinbase-pro", symbol="BTC-USD", description="BTC/USD", security_type=SPOT, currency="USD", settlement_currency="BTC", commission_currency="BTC", tick_size=0.010000000000000002, multiplier=nan, min_trade_vol=0.0010000000000000002, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=0[86400]s, settlement_date=0[86400]s, expiry_datetime=0s, expiry_datetime_utc=0s}
I0413 15:59:31.526456 19791278 strategy.cpp:83] [1:coinbase-pro] ReferenceData={stream_id=1, exchange="coinbase-pro", symbol="BTC-GBP", description="BTC/GBP", security_type=SPOT, currency="GBP", settlement_currency="BTC", commission_currency="BTC", tick_size=0.010000000000000002, multiplier=nan, min_trade_vol=0.0010000000000000002, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=0[86400]s, settlement_date=0[86400]s, expiry_datetime=0s, expiry_datetime_utc=0s}
I0413 15:59:31.526554 19791278 strategy.cpp:83] [1:coinbase-pro] ReferenceData={stream_id=1, exchange="coinbase-pro", symbol="BTC-EUR", description="BTC/EUR", security_type=SPOT, currency="EUR", settlement_currency="BTC", commission_currency="BTC", tick_size=0.010000000000000002, multiplier=nan, min_trade_vol=0.0010000000000000002, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=0[86400]s, settlement_date=0[86400]s, expiry_datetime=0s, expiry_datetime_utc=0s}
I0413 15:59:31.526680 19791278 strategy.cpp:49] [0:deribit] Settings={mbp_max_depth=0, mbp_allow_price_inversion=false, mbp_allow_fractional_tick_size=true}
I0413 15:59:31.526700 19791278 strategy.cpp:33] [0:deribit] DownloadBegin={account=""}
I0413 15:59:31.526722 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=1, account="A1", supports=0x3f0000, status=READY, type=FIX, priority=PRIMARY}
I0413 15:59:31.526744 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=2, account="A1", supports=0x10400000, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.526832 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=3, account="", supports=0x6, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.526865 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=4, account="", supports=0x69, status=READY, type=FIX, priority=PRIMARY}
I0413 15:59:31.526886 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=5, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.526914 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=6, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 15:59:31.526938 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=7, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 15:59:31.526972 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=8, account="", supports=0x68, status=READY, type=FIX, priority=PRIMARY}
I0413 15:59:31.527001 19791278 strategy.cpp:91] [1:coinbase-pro] MarketStatus={stream_id=1, exchange="coinbase-pro", symbol="BTC-USD", trading_status=OPEN}
I0413 15:59:31.527040 19791278 strategy.cpp:91] [1:coinbase-pro] MarketStatus={stream_id=1, exchange="coinbase-pro", symbol="BTC-GBP", trading_status=OPEN}
I0413 15:59:31.527059 19791278 strategy.cpp:91] [1:coinbase-pro] MarketStatus={stream_id=1, exchange="coinbase-pro", symbol="BTC-EUR", trading_status=OPEN}
I0413 15:59:31.527122 19791278 strategy.cpp:41] [1:coinbase-pro] DownloadEnd={account="", max_order_id=0}
I0413 15:59:31.527297 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=9, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.527495 19791278 strategy.cpp:57] [0:deribit] StreamStatus={stream_id=10, account="", supports=0x4, status=READY, type=WEB_SOCKET, priority=PRIMARY}
I0413 15:59:31.527561 19791278 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-24SEP21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18621[86400]s, settlement_date=0[86400]s, expiry_datetime=1632470400s, expiry_datetime_utc=1632470400s}
I0413 15:59:31.527614 19791278 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-31DEC21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18669[86400]s, settlement_date=0[86400]s, expiry_datetime=1640937600s, expiry_datetime_utc=1640937600s}
I0413 15:59:31.527713 19791278 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-23APR21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18719[86400]s, settlement_date=0[86400]s, expiry_datetime=1619164800s, expiry_datetime_utc=1619164800s}
I0413 15:59:31.528996 19791278 strategy.cpp:83] [0:deribit] ReferenceData={stream_id=4, exchange="deribit", symbol="BTC-25JUN21", description="future", security_type=FUTURES, currency="USD", settlement_currency="USD", commission_currency="BTC", tick_size=0.5, multiplier=10, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=nan, underlying="", time_zone="", issue_date=18620[86400]s, settlement_date=0[86400]s, expiry_datetime=1624608000s, expiry_datetime_utc=1624608000s}
I0413 15:59:31.530243 19791278 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=5, exchange="deribit", symbol="BTC-25JUN21", trading_status=OPEN}
I0413 15:59:31.530863 19791278 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-31DEC21", trading_status=OPEN}
I0413 15:59:31.531106 19791278 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-23APR21", trading_status=OPEN}
I0413 15:59:31.531215 19791278 strategy.cpp:91] [0:deribit] MarketStatus={stream_id=3, exchange="deribit", symbol="BTC-24SEP21", trading_status=OPEN}
I0413 15:59:31.537047 19791278 strategy.cpp:41] [0:deribit] DownloadEnd={account="", max_order_id=0}
...
```

Noteworthy observations

* Download is per gateway and must be managed as such. In particular, it is
  possible that multiple downloads can simultaneously be in progress.
