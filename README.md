# How to use the trading interface

Copyright (c) 2017-2018, Hans Erik Thrane

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)


## Contact

You should [contact us](mailto:info@roq-trading.com) if you want to

* Obtain a gateway license key.
* Discuss a specific gateway implementation.
* Discuss bespoke solutions, e.g. consultancy or joint-venture.


## Introduction

This repository contains examples.

* Market data listener demonstrating how to aggregate batch updates and write the result to `stdout`.
* Simple example demonstrating everything needed to implement a market taking strategy.
* Generic reference strategy skeleton demonstrating components typically involved when writing an actual trading strategy.


### Collect

The most basic example demonstrating how to work with the event handlers.

The example shows how to aggregate multiple updates (events) arriving as a batch.
The example also shows how the same strategy can be connected to a gateway or used with an in-process simulator.


### Simple

Demonstrating a very simple market taker strategy.

The strategy looks for jumps as measured by a simple mid-price model.
Reversal is expected if the jump exceeds a configured threshold in which case an IOC order may be sent.
The example also tries to deal with typical open/close considerations normally found in Chinese markets.


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

Feel free to [contact us](mailto:info@roq-trading.com) if you need help with implementing your own strategy.


## Conda

Here we demonstrate only very basic usage of Conda environments.

Please refer to the Conda documentation for further details on how to
[manage environments](https://conda.io/docs/user-guide/tasks/manage-environments.html).


### Basic Steps

Download the latest [Miniconda](https://conda.io/miniconda.html) installer

	# download the miniconda installer
	wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

Create the Conda environment


	# install miniconda (to your home directory)
	bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

	# define conda channels
	cat > ~/miniconda3/.condarc << EOF
	channels:
	  - defaults
	  - https://roq-trading.com/conda/unstable
	EOF

	# install git
	~/miniconda3/bin/conda install -y git

Clone this project (from github) and compile from source

	# clone from github
	git clone https://github.com/roq-trading/roq-samples

	# change to the newly created directory
	cd roq-samples

	# update submodules
	git submodule update --init --recursive

	# install the build toolchain
	scripts/create_conda_env.sh

	# activate your conda environment
	source ~/miniconda3/bin/activate

	# configure the project
	cmake -DCMAKE_BUILD_TYPE=Debug

	# compile
	make -j4

	# configure the project
	cmake -DCMAKE_BUILD_TYPE=Debug

	# compile
	make -j4

Test if it works

	# test simulation
	cd src/collector
	./roq-samples-collector --simulation-file test.csv

*Note! The environment variables will be lost if you use `conda install` again.
If your environment variables have been replaced, simply run `source scripts.conda.sh` (without install)*.

### Updating

Sometimes you may get away with simply doing this

	# update all conda packages
	conda update --all

These are the steps needed to resync your conda environment

	# get the latest conda package
	conda update roq

	# merge the latest samples code
	git pull

	# set CPPFLAGS, LDFLAGS and PKG_CONFIG_PATH
	source scripts/conda.sh

	# update git submodules
	git submodule update --init --recursive

	# configure the project
	cmake -DCMAKE_BUILD_TYPE=Debug

	# compile
	make -j4


### Ubuntu (system-wide build toolchain)

We *strongly* suggest using Conda's build toolchain to avoid ABI incompatibilities.
You really don't need to use system-wide build tools.
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

