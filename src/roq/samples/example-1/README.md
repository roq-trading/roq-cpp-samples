# Example 1

Demonstrates very basic event processing.

* Define the regex patterns used for subscribing market data
* Use the Trader interface to automatically asynchronously manage connections
  and dispatch events
* Use the asynchronous logger to print relevant incoming events


## Prerequisites

Please refer to the top-level [README](../../../README.md) to install, configure
and launch the required market gateways.


## Using

The example requires the `name` option specifying the login name used
when accessing the market gateway(s).

Further arguments are UNIX domain socket end-points for the market gateways.

```bash
./roq-samples-example-1 \
    --name "example-1" \
    ~/deribit.sock \
    ~/coinbase-pro.sock
```

> Verbose logging can be enabled by setting the `ROQ_v` environment
> variable to a non-zero value, e.g. by prefixing your command line
> with `ROQ_v=1`.

> The paths required for the UNIX domain sockets will depend on how
> you configure and launch your market gateways.
