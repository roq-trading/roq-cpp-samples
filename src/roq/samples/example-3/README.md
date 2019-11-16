# Example 3

## Objective

* Demonstrates how to place limit orders upon
  model signal

## What does it do?

* Extends `example-2`
* Keeps track of order updates and maintain
  long/short positions
* Places limit orders, subject to position,
  when the model generates signals

The model tries to detect sharp moves (perhaps
significant order flow) and will generate a signal
when the directional move reverts.

> The model is very simplistic and the parameters
> have not been optimized:
> don't use this for any real trading!


## Requirements

The example requires access to a market gateway
running on the same host.

## How to use

Historical simulation (back-testing) is very useful
to verify the performance of a trading strategy

For this example, make sure you have installed the
Deribit sample dataset

```bash
conda install -y roq-data
```

For this example, simulation is triggered by
providing the `--simulation` flag and a path
to a captured event log

```bash
./roq-samples-example-3 --name "example-3" \
    --simulation \
    $CONDA_PREFIX/share/roq/data/deribit.roq
```

You can enable the placement of limit orders by
adding the `--enable-trading` flag

```bash
./roq-samples-example-3 --name "example-3" \
    --simulation \
    $CONDA_PREFIX/share/roq/data/deribit.roq \
    --enable-trading
```

Finally, connecting to a gateway is similar
to previous examples

```bash
./roq-samples-example-3 --name "example-3" \
    ~/deribit.sock
```

Add the `--enable-trading` flag if you want
orders to be sent to the market.
