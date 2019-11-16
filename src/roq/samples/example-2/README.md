# Example 2

## Objective

* Demonstrate how to maintain state and update a simple model

## What does it do?

* Extends `example-1`
* Cache instrument specific information (such as tick size)
* Process MarketByPrice and maintain a view of depth
* Compute weighted average price and exponential moving average

## Requirements

The example requires access to one or more market gateways running
on the same host.

## How to use

Example:

```bash
./roq-samples-example-2 --name "example-2" \
    ~/deribit.sock \
    ~/coinbase-pro.sock
```
