
```bash
./roq-samples-io-context \
  --name trader \
  --ws_port 2345 \
  --udp_port 1234 \
  ~/run/deribit.sock
```

```bash
nc -u -l 1234
```

```bash
./test.py
```
