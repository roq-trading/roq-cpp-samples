# Roq Trading Solutions

Solutions focused on development, testing and deployment of
algorithmic trading strategies.


## Overview

Topics

* Create a development environment
* Application design
* Handle asynchronous events
* Typical strategy patterns

## What is it?

Examples designed to demonstrate various concepts relating to
the implementation of automated trading strategies.

A very naive implementation would follow this pattern

1. Receive market data update
2. Update model
3. When conditions are met, send order creation request

The reality is unfortunately more complicated.

These are some of the reasons why you have to be careful
when deploying algorithmic trading strategies

* Latest market data snapshot becomes stale if the gateway
  loses connection to the market data feed.
  Client must ensure invalidation of market data (and model
  results) if it receives a disconnect event from the gateway.
* Several market data update events could arrive as a batch.
  Certain trading strategies, e.g. spread trading, will
  benefit from postponing the processing until the entire
  batch has been received.
* Order action requests could get lost in transit.
  It is important to monitor the age of a request and take
  appropriate action if an ack has not been received within
  a timeout period.
  There are no silver bullets: Appropriate action could
  be to simply resend the order action request.
  Or perhaps the right approach is to terminate the program
  and require human intervention.
  (It really depends on your use-case).
* Download must have completed before trading is allowed.
  In particular, accurate positions and working orders
  must have been received before the trading strategy is
  allowed to send new order action requests.

The source code samples has been documented to help
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
  - conda-forge
EOF
```

### Activate Miniconda

> *This section will demonstrate how to activate your Conda environment*.

This is how you activate your Conda environment

```bash
source ~/miniconda3/bin/activate
```

> The following sections will assume you have *already* activated your
> Conda environment.

### Install git

> You don't need this step if a recent version of git is
> available on your system.

```bash
conda install -y git
```

### Clone roq-samples

```bash
# clone roq-samples
git clone https://github.com/roq-trading/roq-samples

# change into the new directory
cd roq-samples
```

### Create a build environment

The project includes a `create_conda_env.sh` helper script which will

* install the required compiler toolchain,
* install required dependencies (Conda packages), and
* configure your Conda environment

```bash
./create_conda_env.sh
```

If all goes well, you will be prompted to start or restart Miniconda.

> Instructions are written to the terminal so you can easily copy-paste.
> For example, something like this:
> 
> ```
> Please re-activate your Conda environment now!
> conda deactivate && source ~/miniconda3/bin/activate
> ```

### Update git submodules

Use the helper script to update git submodules used by the project

```bash
./git_init_submodules.sh
```

### Build the Project

```bash
cmake -DCMAKE_AR="$AR" -DCMAKE_RANLIB="$RANLIB" -DCMAKE_NM="$NM" -DCMAKE_BUILD_TYPE=Debug
make -j4
```

### Run an in-process simulation

> This script will write simulation results (csv files) to current directory.

```bash
src/roq/samples/simple/simulate.sh
```

You can also increate the logging verbosity if you want to see more details

```bash
ROQ_v=1 src/roq/samples/simple/simulate.sh
```

### Install the simulator service

> The `roq-data` package is not required: it provides a default dataset
> useful for demonstrating the simulator.
> It has probably already been installed by the `create_conda_env.sh`
> helper script mentioned earlier.

```bash
conda install -y roq-simulator roq-data
```

#### Launch the simulator service

```bash
roq-simulator \
  --name "simulator" \
  --dispatcher-affinity 1 \
  --simulator-affinity 2 \
  --buffer-size 266240 \
  --config-directory "$CONDA_PREFIX/share/roq/simulator" \
  --config-file "master.conf" \
  --listen "$HOME/simulator.sock" \
  --metrics 1234 \
  --generator-type "csv-old" \
  "$CONDA_PREFIX/share/roq/data/USTP_Multi_20180223.csv"
```

#### Run the strategy against the simulator service

> You probably want to run the next command from a new terminal.
> Remember to activate your Conda environment.

```bash
src/roq/samples/simple/run.sh $HOME/simulator.sock
```

#### Run the strategy against a gateway

Same as previous step only using the socket exposed by the gateway.

### Summary

You have managed to achieve the following

* Create a Conda environment
* Clone the `roq-samples` project
* Install relevant build tools
* Compile the `roq-samples` project
* Run the strategy as an in-process simulation
* Run the strategy against a simulator service

> Compiled binaries are compatible with Conda packages.
> You can use existing Conda packages without having to
> understand how to build those dependencies.
> That's one of the benefits of using a package manager.

### Making your own project

Easiest is to fork the `roq-samples` project and then modify
as you see fit.

The better approach is to understand the build
configuration and the code.
You should then, from scratch, create your own build configuration.

## Next steps

The different sub-projects are meant to demonstrate various concepts.

The most trivial project is the
[collector](https://github.com/roq-trading/roq-samples/tree/master/src/roq/samples/collector).
It subscribes to market data and writes anything it receives to the log.

The [simple](https://github.com/roq-trading/roq-samples/tree/master/src/roq/samples/simple)
project demonstrates the various event handlers.
It also implements trivial patterns to manage e.g. monitoring for order action
timeout.

The [common](https://github.com/roq-trading/roq-samples/tree/master/src/roq/samples/common)
project is a library meant to create a higher level interface to implement
a trivial market taker application.

The [taker](https://github.com/roq-trading/roq-samples/tree/master/src/roq/samples/taker)
project uses the common library to implement a trivial market taking strategy.

The [maker](https://github.com/roq-trading/roq-samples/tree/master/src/roq/samples/maker)
project uses the common library to implement a trivial market making strategy.

* [Contact us](mailto:info@roq-trading.com)
* [Roq Trading Solutions (website)](https://roq-trading.com)
* [Online documentation](https://roq-trading.com/docs)
* [API](https://github.com/roq-trading/roq-api)
* [Ansible playbook](https://github.com/roq-trading/roq-ansible)
* [Grafana dashboards](https://github.com/roq-trading/roq-grafana)
* [Vagrant development environments](https://github.com/roq-trading/roq-vagrant)
