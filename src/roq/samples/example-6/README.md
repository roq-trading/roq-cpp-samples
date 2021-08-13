# Example 6

Demonstrates how to use several event-logs.


## Prerequisites

Please refer [here](../../../../README.md#gateways-install-configure-run) to
install, configure and run the required market gateways.


## Using

For this example, make sure you have installed the `roq-data` conda package.

### Simulation

Simulation is controlled by the `--simulation=true` flag

```bash
./roq-samples-example-6 \
    --name "trader" \
    --simulation=true \
    $CONDA_PREFIX/share/roq/data/deribit.roq \
    $CONDA_PREFIX/share/roq/data/bitmex.roq
```

### Live Trading

Switching to live trading

```bash
./roq-samples-example-3 \
    --name "trader" \
    ~/deribit.sock \
    ~/bitmex.sock
```
