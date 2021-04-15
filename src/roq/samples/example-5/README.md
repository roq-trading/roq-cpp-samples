# Example 5

Demonstrates how to manage a secondary thread and how to pass a `CustomMessage`
from the secondary thread to the main dispatch loop.


## Prerequisites

Please refer [here](../../../../README.md#gateways-install-configure-run) to
install, configure and run the required market gateways.


## Using

The example requires the `name` option specifying the login name used
when accessing the market gateway(s).

Further arguments are UNIX domain socket end-points for the market gateways.

### Single Gateway

```bash
./roq-samples-example-5 \
    --name "trader" \
    ~/deribit.sock
```

```text
I0415 05:41:44.053465 258955 service.cpp:50] ===== START =====
I0415 05:41:44.055003 258955 service.cpp:34] The metrics service will *not* be started
I0415 05:41:44.061089 258955 controller.cpp:74] session_id="69c87294-fc6f-4c5e-b284-bf429e1d066d"
I0415 05:41:44.061113 258955 controller.cpp:78] Dispatching...
I0415 05:41:44.061141 258955 controller.cpp:82] Starting event loop thread...
I0415 05:41:44.061580 258957 producer.cpp:32] producer was started
I0415 05:41:44.061568 258956 controller.cpp:113] Event loop thread is now running
I0415 05:41:44.061781 258955 pollster.cpp:334] Register consumer
I0415 05:41:44.061808 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.161879 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.261975 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.362072 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.462175 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.562272 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.662368 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.762466 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.862563 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
I0415 05:41:44.962661 258955 strategy.cpp:38] [0:internal-6751528361785890386] CustomMessage={length=12}
^CW0415 05:41:45.011017 258956 controller.cpp:123] Signal 2 (Interrupt)
I0415 05:41:45.011028 258956 controller.cpp:119] Event loop thread has terminated
I0415 05:41:45.062882 258957 producer.cpp:42] producer was terminated
I0415 05:41:45.062943 258955 controller.cpp:88] Waiting for event loop thread to terminate...
I0415 05:41:45.062958 258955 controller.cpp:91] Done!
I0415 05:41:45.063070 258955 service.cpp:54] ===== STOP =====
```
