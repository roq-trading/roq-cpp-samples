# Roq Trading Solutions

Solutions focused on development, testing and deployment of
algorithmic trading strategies.


## Overview

Topics

* Create a development environment.
* Application design.
* Handle asynchronous events.
* Typical strategy patterns.

## What is it?

Examples designed to demonstrate various concepts relating to
the implementation of automated trading strategies.

A very naive implementation would follow this pattern

1. Receive market data update
2. Update model
3. Send order creation request (when conditions are met)

The reality is unfortunately much more complicated!

These are some of the reasons why you have to be careful

* Latest market data snapshot becomes stale if the gateway
  loses connection to the market data feed.
  Client must ensure invalidation of market data if it
  receives a disconnect event.
* Several market data update events could arrive as a batch.
  Certain trading strategies, e.g. spread trading, will
  benefit from postponing the processing until the entire
  batch has been received.
* Order action requests could get lost in transit.
  It is important to monitor for request timeout and take
  appropriate action if an ack has not been received.
  There are no silver bullets: Appropriate action could
  be to simply resend the order action request.
  Or perhaps the right approach is to terminate the program
  and require human intervention.
* Download must be completed before trading is allowed.
  In particular, current positions and existing working orders
  must be known to the trading strategy before it starts
  sending new order action requests.

The source code examples are heavily documented to help
you better understand the motivations and reasons for
specific design patterns.


## First steps

### Install Miniconda

> *This section will demonstrate how to install Miniconda*.

A Conda environment allows you to install up-to-date binary packages
on your development box *without* requiring root access.

```bash
# download the miniconda installer
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

# install miniconda (to your home directory)
bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

# configure roq-trading as a default conda channel
cat > ~/miniconda3/.condarc << EOF
channels:
  - defaults
  - https://roq-trading.com/conda/unstable
EOF
```

### Activate Miniconda

> *This section will demonstrate how to activate your Conda environment*.

This is how you activate your Conda environment

```bash
source ~/miniconda3/bin/activate
```

> The following sections will assume you have *already* activated your
Conda environment.

### Install git

If `git` is not already install on your system

```bash
conda install -y git
```

### Clone the project

```bash
git clone https://github.com/roq-trading/roq-samples
```

### Create a build environment

The project includes a `create_conda_env.sh` script which will

* Install the compiler toolchain
* Install Roq dependency packages
* Configure the Conda environment

```bash
./create_conda_env.sh
```

If all goes well, you will be prompted to start or restart Conda.

### Update git submodules

Use this script to update git submodules used by the project

```bash
./git_init_submodules.sh
```
### Build the Project

```bash
mkdir build
cd build
cmake ..
make -j4
```

### Run a simulation

```bash
cd src/roq/samples/simple
ROQ_v=1 ./simulate.sh
```

### Summary

With the following steps you have achieved the following

* Created a (disposable) Conda environment.
* Cloned the `roq-samples` project.
* Installed relevant build tools.
* Compiled the `roq-samples` project.
* Run a simulation.

> Compiled binaries are compatible with Conda packages.
> You can use existing Conda packages without having to care
> how those packages are built.
> That's one of the benefits of using a package manager.

It is easiest to fork the `roq-samples` project for your own
project.
Of course, the better approach is to understand the code and build
configuration first, then create your own configuration from scratch.

## Next steps

The different sub-projects are meant to demonstrate various concepts.

The most trivial project is the
[collector](https://github.com/roq-trading/roq-samples/src/roq/samples/collector).
It subscribes to market data and writes anything it receives to the log.

The [simple](https://github.com/roq-trading/roq-samples/src/roq/samples/simple)
project demonstrates the various event handlers.
It also implements trivial patterns to manage e.g. monitoring for order action
timeout.

The [common](https://github.com/roq-trading/roq-samples/src/roq/samples/common)
project is a library meant to create a higher level interface to implement
a trivial market taker application.

The [taker](https://github.com/roq-trading/roq-samples/src/roq/samples/taker)
project uses the common library to implement a trivial strategy.

* [Contact us](mailto:info@roq-trading.com)
* [Roq Trading Solutions (website)](https://roq-trading.com)
* [Online documentation](https://roq-trading.com/docs)
* [API](https://github.com/roq-trading/roq-api)
* [Ansible playbook](https://github.com/roq-trading/roq-ansible)
* [Grafana dashboards](https://github.com/roq-trading/roq-grafana)
* [Vagrant development environments](https://github.com/roq-trading/roq-vagrant)
