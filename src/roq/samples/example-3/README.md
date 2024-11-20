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

**!!! DEPRECATED !!!**

> See: [roq-cpp-algo-template](https://github.com/roq-trading/roq-cpp-algo-template)

### Live Trading

Switching to live trading

```bash
./roq-samples-example-3 \
    --name "trader" \
    ~/deribit.sock
```

Then add the `--enable_trading` flag if you really want orders to be placed on
the market.
