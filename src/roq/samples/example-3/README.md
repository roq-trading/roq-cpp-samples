# Example 3

## Objective

* Demonstrate how to maintain positions and place a simple (IOC)
  orders on signal

## What does it do?

* Extends `example-2`
* Maintain positions
* Place IOC order when model generates a signal

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
./roq-samples-example-3 --name test ~/deribit.sock ~/coinbase-pro.sock
```

> *The paths used for the UNIX domain sockets obviously depend on how
> you configure and launch your gateways*.
