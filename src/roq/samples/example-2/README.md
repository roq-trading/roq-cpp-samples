# Example 2

Demonstrates how to maintain state and update a simple model.

* Extends `example-1`
* Cache instrument specific information (such as tick size)
* Process MarketByPrice and maintain a view of depth
* Compute weighted average price and exponential moving average

## Prerequisites

Please refer to the top-level [README](../../../README.md) to install, configure
and launch the required market gateways.


## Using

```bash
./roq-samples-example-2 \
    --name "example-2" \
    ~/deribit.sock \
    ~/coinbase-pro.sock
```
