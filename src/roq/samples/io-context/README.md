Broadcast everything
```bash
./roq-samples-io-context \
  --name trader \
  --ws_port 2345 \
  --udp_port 1234 \
  ~/run/deribit.sock
```

Broadcast controlled from WS subscribe/unsubscribe
```bash
./roq-samples-io-context \
  --name trader \
  --ws_port 2345 \
  --udp_port 1234 \
  --filter_symbols \
  ~/run/deribit.sock
```

Listen to UDP
```bash
nc -u -l 1234
```

WS request (you should modify to your needs)
```bash
./test.py
```
