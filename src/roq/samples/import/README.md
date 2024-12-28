# Import

Demonstrates how to convert any data source into Roq's Event-Log format using
Flatbuffers as an intermediary format.

The general structure is

* `SourceInfo` used to capture sequence number and timestamp
* `GatewaySettings` must be the first message
* `ReferenceData` preferably precedes any market data messages
* `MarketByPriceUpdate` preferably starts with a snapshot (an image)
  followed by incremental updates (changes)

> This example only depends on headers available from 'roq-api'.
> There are **no** requirements  to link against `roq-logging` or `roq-client`
> in order to encode Flatbuffers messages using the schema dfined in `roq-api`.
> That being said, this example has been set up to link against `roq-logging`
> mainly for convenience (logging, flags, etc.).


## Prerequisites

You don't need a Conda environment to get the required headers.
It could be as simple as cloning `roq-api` from GitHub

```bash
git clone https://github.com/roq-trading/roq-api
```

> You would then need to update your project include directories as appropriate.

If you're comfortable using Conda (ABI compatibility, etc.), it may be more
convenient to simply install the `roq-api` package

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-api
```

For this example, we rely on the more complete Conda environment as described
 [here](../../../../README.md#prerequisites).

For the following section, you may want to also install the `roq-tools` package

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-tools
```


## Using

### Convert any Data-Source to a Flatbuffers Stream

```bash
./roq-samples-import \
    --encoding binary \
    my_tmp_file
```

### Convert a Flatbuffers Stream to the Event-Log Format

```bash
roq-import \
    --type event_log \
    --protocol flatbuffers \
    --encoding binary \
    --name cme \
    --event_log_dir . \
    my_tmp_file
```

### Inspect Generated Event-Log

Summary like this

```bash
roq-dump \
    --summary true \
    cme/test.roq
```

```console
+--------------------------+------------------+------------------+------------+
| type                     |            count |            bytes |  avg. size |
+--------------------------+------------------+------------------+------------+
| subscribe                |                0 |                0 |        nan |
| download_begin           |                0 |                0 |        nan |
| download_end             |                0 |                0 |        nan |
| gateway_settings         |                1 |                0 |        0.0 |
| external_latency         |                0 |                0 |        nan |
| market_data_status       |                0 |                0 |        nan |
| order_manager_status     |                0 |                0 |        nan |
| reference_data           |                1 |              128 |      128.0 |
| market_status            |                1 |              304 |      304.0 |
| top_of_book              |                0 |                0 |        nan |
| market_by_price_update   |                2 |              520 |      260.0 |
| market_by_order_update   |                0 |                0 |        nan |
| trade_summary            |                0 |                0 |        nan |
| statistics_update        |                0 |                0 |        nan |
| create_order             |                0 |                0 |        nan |
| modify_order             |                0 |                0 |        nan |
| cancel_order             |                0 |                0 |        nan |
| order_ack                |                0 |                0 |        nan |
| order_update             |                0 |                0 |        nan |
| trade_update             |                0 |                0 |        nan |
| position_update          |                0 |                0 |        nan |
| funds_update             |                0 |                0 |        nan |
| custom_message           |                0 |                0 |        nan |
+--------------------------+------------------+------------------+------------+
| total                    |                5 |              952 |      190.4 |
+--------------------------+------------------+------------------+------------+
controller.cpp:741] period 1ns to 5ns
controller.cpp:742] duration 0d 0h 0m 0s
```

Details like this

```bash
roq-dump \
    --type log \
    cme/test.roq
```

```console
controller.cpp:102] event={message_info={source=0, source_name="cme", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=1, receive_time_utc=1ns, receive_time=1ns, source_send_time=1ns, source_receive_time=1ns, origin_create_time=1ns, origin_create_time_utc=1ns, is_last=true, opaque=0}, gateway_settings={mbp_max_depth=3, mbp_allow_price_inversion=false}}
controller.cpp:179] event={message_info={source=0, source_name="cme", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=2, receive_time_utc=2ns, receive_time=2ns, source_send_time=2ns, source_receive_time=2ns, origin_create_time=2ns, origin_create_time_utc=2ns, is_last=true, opaque=0}, reference_data={exchange="CME", symbol="GEZ1", description="", security_type=UNDEFINED, currency="", settlement_currency="", commission_currency="", tick_size=0.0025, multiplier=2500, min_trade_vol=1, option_type=UNDEFINED, strike_currency="", strike_price=0, underlying="", time_zone="", issue_date=0[86400]s, settlement_date=0[86400]s, expiry_datetime=0s, expiry_datetime_utc=0s}}
controller.cpp:218] event={message_info={source=0, source_name="cme", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=3, receive_time_utc=3ns, receive_time=3ns, source_send_time=3ns, source_receive_time=3ns, origin_create_time=3ns, origin_create_time_utc=3ns, is_last=true, opaque=0}, market_status={exchange="CME", symbol="GEZ1", trading_status=OPEN}}
controller.cpp:269] event={message_info={source=0, source_name="cme", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=4, receive_time_utc=4ns, receive_time=4ns, source_send_time=4ns, source_receive_time=4ns, origin_create_time=4ns, origin_create_time_utc=4ns, is_last=true, opaque=0}, market_by_price_update={exchange="CME", symbol="GEZ1", bids=[{price=99.785, quantity=3}, {price=99.78, quantity=2}, {price=99.775, quantity=1}], asks=[{price=99.8, quantity=3}, {price=99.805, quantity=2}, {price=99.81, quantity=1}], snapshot=true, exchange_time_utc=0ns}}
controller.cpp:269] event={message_info={source=0, source_name="cme", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=5, receive_time_utc=5ns, receive_time=5ns, source_send_time=5ns, source_receive_time=5ns, origin_create_time=5ns, origin_create_time_utc=5ns, is_last=true, opaque=0}, market_by_price_update={exchange="CME", symbol="GEZ1", bids=[{price=99.785, quantity=0}, {price=99.77, quantity=2}], asks=[], snapshot=false, exchange_time_utc=0ns}}
```

### Simulation

You could also try the simulation example

```bash
ROQ_v=1 ../example-3/roq-samples-example-3 \
    --name trader \
    --exchange CME \
    --symbol GEZ1 \
    --simulation \
    cme/test.roq
```

```console
I0208 06:41:31.840719 396449 service.cpp:66] ===== START =====
I0208 06:41:31.848948 396449 controller.cpp:108] StartEvent={message_info={source=0, source_name="", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=0, receive_time_utc=0ns, receive_time=0ns, source_send_time=0ns, source_receive_time=0ns, origin_create_time=0ns, origin_create_time_utc=0ns, is_last=false, opaque=0}, start={}}
I0208 06:41:31.848969 396449 controller.cpp:115] ConnectionEvent {message_info={source=0, source_name="CME", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=0, receive_time_utc=0ns, receive_time=0ns, source_send_time=0ns, source_receive_time=0ns, origin_create_time=0ns, origin_create_time_utc=0ns, is_last=true, opaque=0}, connection={status=CONNECTED}}
I0208 06:41:31.848977 396449 instrument.cpp:46] [CME:GEZ1] connection_status=CONNECTED
I0208 06:41:31.849061 396449 instrument.cpp:110] [CME:GEZ1] tick_size=0.0025
I0208 06:41:31.849067 396449 instrument.cpp:113] [CME:GEZ1] min_trade_vol=1
I0208 06:41:31.849069 396449 instrument.cpp:117] [CME:GEZ1] multiplier=2500
I0208 06:41:31.849077 396449 instrument.cpp:128] [CME:GEZ1] trading_status=OPEN
I0208 06:41:31.849356 396449 instrument.cpp:148] [CME:GEZ1] depth=[{bid_price=99.785, bid_quantity=3, ask_price=99.8, ask_quantity=3}, {bid_price=99.78, bid_quantity=2, ask_price=99.805, ask_quantity=2}, {bid_price=99.775, bid_quantity=1, ask_price=99.81, ask_quantity=1}]
I0208 06:41:31.849369 396449 controller.cpp:132] ConnectionEvent {message_info={source=0, source_name="CME", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=0, receive_time_utc=0ns, receive_time=0ns, source_send_time=0ns, source_receive_time=0ns, origin_create_time=0ns, origin_create_time_utc=0ns, is_last=true, opaque=0}, connection={status=DISCONNECTED}}
I0208 06:41:31.849372 396449 instrument.cpp:46] [CME:GEZ1] connection_status=DISCONNECTED
I0208 06:41:31.849378 396449 controller.cpp:139] StopEvent={message_info={source=0, source_name="", source_session_id="00000000-0000-0000-0000-000000000000", source_seqno=0, receive_time_utc=0ns, receive_time=0ns, source_send_time=0ns, source_receive_time=0ns, origin_create_time=0ns, origin_create_time_utc=0ns, is_last=false, opaque=0}, stop={}}
I0208 06:41:31.850027 396449 service.cpp:80] ===== STOP =====
```
