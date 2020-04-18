# Roq Trading Solutions

*A C++ HFT Toolkit for Algo Traders*


## What is it?

A collection of examples meant to demonstrate
some features of 
`roq-client` and
[`roq-api`](https://github.com/roq-trading/roq-api).


## Overview

The examples will demonstrate the increasing levels
of complexity required to implement a real, fully
autonomous, trading strategy

* [Example 1](./src/roq/samples/example-1/README.md)
  * Connect to market gateway
  * Subscribe using regex patterns
* [Example 2](./src/roq/samples/example-2/README.md)
  * Manage disconnect
  * Process incremental market data update
  * Maintain a market depth view
  * Update a simple model
* [Example 3](./src/roq/samples/example-3/README.md)
  * Maintain positions
  * Place limit orders
  * Deal with order acks and updates
  * Historical simulation
  * Live trading


## Constraints

* **Linux**
  * RHEL, CentOS, Debian, Ubuntu
* **C++17**
  * Conda's GCC

## Conda

Download the Miniconda Installer

```
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
```

Install Miniconda

```
bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3
```

Add roq-trading as a channel

```
cat > ~/miniconda3/.condarc << EOF
channels:
  - https://roq-trading.com/conda/stable
  - defaults
  - conda-forge
EOF
```

Activate Miniconda

```
source ~/miniconda3/bin/activate
```

Install dev tools

```
conda install -y git cmake gxx_linux-64 gdb_linux-64
```

## Build

Update git submodules

```
git submodule update --init --recursive
```

Install `roq-client`

```
conda install -y roq-client
```

Run CMake

```
cmake \
    -DCMAKE_AR="$AR" \
    -DCMAKE_RANLIB="$RANLIB" \
    -DCMAKE_NM="$NM" \
    -DCMAKE_BUILD_TYPE=Debug
```

> Remove the CMakeCache.txt file, if you must
> repeat any of the previous steps.

Compile the project

```
make -j4
```

## Dependencies

### Data

Download Roq sample data

```
conda install -y roq-data
```

This will install an "event log" to
`$CONDA_PREFIX/share/roq/data/deribit.roq`.
You can use this for historical simulations.

> Many thanks to Deribit for giving their permission
> to distributing this small dataset.

### Gateway

*A gateway may require a license file.
Please [contact us](mailto:info@roq-trading.com)
if you have any questions*.

#### [Deribit](https://roq-trading.com/docs/gateways/deribit/index.html)

Download the Deribit gateway

```
conda install -y roq-deribit
```

Make a copy of the default configuration template

```
cp $CONDA_PREFIX/share/roq/deribit/config.toml ./deribit.toml
```

You must edit this file and update with your
Deribit API credentials
([link](https://test.deribit.com/main#/account?scrollTo=api)).

You should look for these lines and replace

```
login = "YOUR_DERIBIT_LOGIN_GOES_HERE"
secret = "YOUR_DERIBIT_SECRET_GOES_HERE"
```

The gateway is started like this

```
roq-deribit \
    --name "deribit" \
    --config-file deribit.toml \
    --listen ~/deribit.sock
```

#### [Coinbase Pro](https://roq-trading.com/docs/gateways/deribit/index.html)

Coinbase Pro is almost identical

Install

```
conda install -y roq-coinbase-pro
```

Copy default configuration template

```
cp $CONDA_PREFIX/share/roq/coinbase-pro/config.toml ./coinbase-pro.toml
```

You must edit this file and update with your
Coinbase Pro API credentials
([link](https://public.sandbox.pro.coinbase.com/profile/api)).

```
login = "YOUR_COINBASE_PRO_API_KEY_GOES_HERE"
password = "YOUR_COINBASE_PRO_PASSPHRASE_GOES_HERE"
secret = "YOUR_COINBASE_PRO_SECRET_GOES_HERE"
```

Start

```
roq-coinbase-pro \
    --name "coinbase-pro" \
    --config-file coinbase-pro.toml \
    --listen ~/coinbase-pro.sock
```


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
