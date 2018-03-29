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

The example has the basic (but *not* finalized!) structure for

* Risk management.
* Order management.
* Position management.
* Gateway management.

**Warning!**
This is not a finalized solution!
It is only meant to demonstrate a typical (reference) implementation.

A market-taker strategy may not want to deal with the complexities of limit orders.
Perhaps it only sends IOC (Immediate or Cancel) orders.
The life-time of such orders is fairly easy to manage.
This reference example, with complex risk management, may be overkill for such strategies.

However, a market making strategy wil have to manage the life-time of limit orders.
Such strategies are therefore much more complicated.
Risk management is necessary to protect against inadvertenly sending too many limit orders.

Thus

* The job of the risk manager is to compare existing (sent and live) orders to existing positions and risk limits.
* The job of the order manager is to manage state transitions for (potentially) many orders.
* The job of the position manager is to accumulate fills (trades) as they arrive.
* The job of the gateway manager is to route events.
  On the one side it interacts with the gateways.
  On the other side it interacts with the various components of the trading system.

The example also shows how one should implement the client for live trading or simulation.

Feel free to [contact us](mailto:hans.thrane@roq-trading.com) if you need help with implementing your own strategy.

## Conda

Here we demonstrate only very basic usage of Conda environments.

Please refer to the Conda documentation for further details on how to
[manage environments](https://conda.io/docs/user-guide/tasks/manage-environments.html).

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

	# install roq
	conda install -y roq

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

However, you're (obviously) free to use any other build toolchain as long as it's
ABI compatible with the Roq API downloaded from Conda.
(And if that's still not sufficient, you'll have to resort to building the Roq API from source).

A few package dependencies are required in order to compile on Ubuntu.

* autoconf
* libtool
* pkg-config
* gcc

You may have to run the following command to prepare your system

	sudo apt-get install -y autoconf autoconf-archive libtool pkgconfig gcc

