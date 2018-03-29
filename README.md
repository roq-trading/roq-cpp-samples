# How to use the trading interface

Copyright (c) 2017-2018, Hans Erik Thrane

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)


## Contact

You should [contact us](mailto:hans.thrane@roq-trading.com) if you want to

* Obtain a gateway license key.
* Discuss a specific gateway implementation.
* Discuss bespoke solutions, e.g. consultancy or joint-venture.


## Introduction

This repository contains two examples.
A collector writing market data to `stdout`.
And a reference strategy demonstrating components typically involved when writing an actual trading strategy.

### Collector

The most basic example demonstrating how to work with the event handlers.

The example shows how to aggregate multiple updates (events) arriving as a batch.
The example also shows how one should implement the client for live trading or simulation.


### Reference

Demonstrating how to pass updates (events) between the typical components of a real trading strategy.

The example has the basic structure for

* Risk management.
* Order management.
* Position management.
* Gateway management.

This is not a finalized solution, mostly because each trading strategy works differently.

A market-taker strategy may not want to deal with the complexities of limit orders.
Perhaps it only sends IOC (immediate or cancel) orders.
The life-time of such an order is fairly easy to manage.
The reference example may be overkill for such strategies.

A market making strategy wil have to manage limit orders.
Such a strategy is much more complicated.
A risk manager is necessary to protect against inadvertenly sending too many limit orders.

The job of the risk manager is to compare existing (sent and live) orders to existing positions and risk limits.
The job of the order manager is to manage the order state transitions.
The job of the position manager is to accumulate fills (trades) as they arrive.
The gateway manager maintains the top-level routing between gateways on the one side, and the various components
(just described) on the other side.

The example also shows how one should implement the client for live trading or simulation.

## Conda

Here we demonstrate only very basic usage of Conda environments.
Please refer to the [Conda documentation](https://conda.io/docs/user-guide/tasks/manage-environments.html)
for further details.

### Basic Steps

Download [Miniconda](https://conda.io/miniconda.html)

	wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

Create the Conda environment

	# install miniconda (to your home directory)
	bash Miniconda3-latest-Linux-x86_64.sh -b

	# activate miniconda
	source ~/miniconda3/bin/activate

	# define conda channels
	cat > ~/miniconda3/.condarc << EOF
	channels:
	  - anaconda
	  - http://roq-trading.com/dist/conda/unstable
	EOF

Configure the build environment

	# (optional) install the conda build toolchain
	conda install -y autoconf automake libtool pkgconfig gcc_linux-64

	# (optional) install git
	conda install -y git

	# define environment variables
	export LDFLAGS=-L"$CONDA_PREFIX/lib"
	export CPPFLAGS=-I"$CONDA_PREFIX/include"
	export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"

Install the Roq API

	# install the api
	conda install -y roq-api

	# clone roq-samples (this repo)
	git clone https://github.com/roq-trading/roq-samples
	cd roq-samples

	# configure the project
	./autogen.sh && ./configure --prefix "$CONDA_PREFIX"

	# compile
	make -j4

	# test
	cd src/collector
	./example-collector --mode simulation --simulation-file SOME_FILE

### Ubuntu (system-wide build toolchain)

We *strongly* suggest using Conda's build toolchain to avoid ABI incompatibilities.
This section is only shown for completeness.

However, you're free to use any other build toolchain as long as it's ABI compatible with
the Roq API downloaded from Conda.

A few dependencies are then required in order to compile on Ubuntu.
You may have to run the following command to prepare the system

	sudo apt-get install -y autoconf autoconf-archive libtool pkgconfig gcc

