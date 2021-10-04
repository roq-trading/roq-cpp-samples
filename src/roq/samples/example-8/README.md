# Example 8

Demonstrates conservative measurement of queue position of limit order.

## Prerequisites

Please refer [here](../../../../README.md#gateways-install-configure-run) to
install, configure and run the required market gateways.


## Using

The example requires the `name` option specifying the login name used
when accessing the market gateway(s).

Further arguments are UNIX domain socket end-points for the market gateways.

```bash
./roq-samples-example-8 \
    --name "trader" \
    ~/deribit.sock
```
