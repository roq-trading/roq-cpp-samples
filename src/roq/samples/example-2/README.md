# Example 2

## Objective

* Demonstrate how to maintain state and update a simple model

## What does it do?

* Extends `example-1`
* Cache instrument specific information (such as tick size) eventually
  required for placing orders
* Process MarketByPrice and maintain depth
* Compute weighted average price and exponential moving average

## Requirements

The example requires access to one or more market gateways running
on the same host.

> *The simulator can also be used as a gateway*.

## How to use

The example requires the `name` option specifying the login name used
when accessing the gateway(s).

Further arguments are UNIX domain socket end-points for the gateways.

Example:

```bash
./roq-samples-example-2 --name test ~/deribit.sock ~/coinbase-pro.sock
```

> *The paths used for the UNIX domain sockets obviously depend on how
> you configure and launch your gateways*.
