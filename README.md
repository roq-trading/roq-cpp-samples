# Roq Trading Solutions

*C++ HFT Toolkit for Algo Traders*.


## Overview

This project demonstrates how to use the
[Roq API](https://github.com/roq-trading/roq-api).

* [Example 1](./src/roq/samples/example-1/README.md)
  demonstrates how to connect to a market gateway and
  subscribe to updates
* [Example 2](./src/roq/samples/example-1/README.md)
  demonstrates how to deal with disconnects, process
  incremental market data updates, maintain a market
  depth view and update a simple model.
* [Example 3](./src/roq/samples/example-1/README.md)
  demonstrates how to maintain positions, place limit
  orders, deal with order acks and order updates,
  historical simulation and live trading.


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
  - https://roq-trading.com/conda/unstable
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

Install the Roq client library

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

#### Deribit

Download the Deribit gateway

```
conda install -y roq-deribit
```

Make a copy of the default configuration template

```
cp $CONDA_PREFIX/share/roq/deribit/config.toml .
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
    --config-file config.toml \
    --listen ~/deribit.sock
```

#### Coinbase Pro

Coinbase Pro is almost identical

Install

```
conda install -y roq-coinbase-pro
```

Copy default configuration template

```
cp $CONDA_PREFIX/share/roq/coinbase-pro/config.toml .
```

Edit and replace

```
login = "YOUR_COINBASE_PRO_API_KEY_GOES_HERE"
password = "YOUR_COINBASE_PRO_PASSPHRASE_GOES_HERE"
secret = "YOUR_COINBASE_PRO_SECRET_GOES_HERE"
```

Start

```
roq-coinbase-pro \
    --name "coinbase-pro" \
    --config-file config.toml \
    --listen ~/coinbase-pro.sock
```


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
