Broadcast everything
```bash
./roq-samples-io-context \
  --name trader \
  --ws_port 2345 \
  --udp_port 1234 \
  ~/run/deribit.sock
```

List of symbols controlled by Rest/WS subscribe/unsubscribe requests
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

Send WS requests (modify to your needs)
```bash
./test_ws.py
```

Send Rest requests (modify to your needs)
```bash
./test_rest.sh
```
