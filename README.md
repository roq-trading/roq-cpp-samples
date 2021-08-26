# roq-samples

Samples demonstrating how to implement various features of algorithmic trading
solutions.


## Operating Systems

* Linux (x86-64, AArch64)
* macOS (x86-64)

*Note!*
The Windows Subsystem for Linux (WSL) is **NOT** supported.
Reason can be found [here](https://devblogs.microsoft.com/commandline/windowswsl-interop-with-af_unix/).


## Library/Package Dependencies

* [Abseil-C++](https://github.com/abseil/abseil-cpp) (Apache 2.0 License)
* [FlatBuffers](https://github.com/google/flatbuffers) (Apache 2.0 License)
* [fmt](https://github.com/fmtlib/fmt) (MIT License)
* [range-v3](https://github.com/ericniebler/range-v3) (BSL 1.0 License)
* [span-lite](https://github.com/martinmoene/span-lite) (BSL 1.0 License)
* [roq-api](https://github.com/roq-trading/roq-api) (MIT License)
* [roq-logging](https://github.com/roq-trading/roq-api) (MIT License)
* roq-client (Commerical License, free to use)

Optional

* [Google Benchmark](https://github.com/google/benchmark) (Apache 2.0 License)
* [Google Test](https://github.com/google/googletest) (BSD 3-Clause License)


## Prerequisites

The project is primarily designed to be compatible with the conda package manager.

### Download and Install Miniforge

#### Linux / x86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-x86_64.sh

bash Miniforge3-Linux-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    gxx_linux-64
```

#### Linux / aarch64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-aarch64.sh

bash Miniforge3-Linux-aarch64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    gxx_linux-aarch64
```

#### macOS / x86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-MacOSX-x86_64.sh

bash Miniforge3-MacOSX-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
   clang_osx-64
```

### Install Dependencies

```bash
source ~/conda/bin/activate

conda install -y \
    git \
    make \
    cmake \
    abseil-cpp

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-client
```

## Building

```bash
git submodule update --init --recursive

cmake .

make -j4
```


## Using

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
* [Example 4](./src/roq/samples/example-4/README.md)
  * Subscribe and nothing else
* [Example 5](./src/roq/samples/example-5/README.md)
  * Transfer `CustomMessage` from a secondary thread
* [Import](./src/roq/samples/import/README.md)
  * Convert any data source to an event-log
  * Encoding has header-only dependencies


### Simulation Data (install)

Simulation requires you to either use your own event logs (automatically
captured when you're running a gateway) or use sample data

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-data
```

Data can now be found in the `$CONDA_PREFIX/share/roq-data/` directory.


### Gateways (install, configure, run)


#### [Deribit](https://roq-trading.com/docs/gateways/deribit/index.html)

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-deribit
```

It is easiest to start from a config file template

```bash
cp $CONDA_PREFIX/share/roq-deribit/config.toml ~/deribit.toml
```

Edit this file and update with your Deribit API credentials
([link](https://test.deribit.com/main#/account?scrollTo=api)).

You should look for these lines and replace as appropriate

```text
login = "YOUR_DERIBIT_LOGIN_GOES_HERE"
secret = "YOUR_DERIBIT_SECRET_GOES_HERE"
```

Launch the gateway

```bash
roq-deribit \
    --name "deribit" \
    --config_file ~/deribit.toml \
    --client_listen_address ~/deribit.sock
```

#### [Coinbase Pro](https://roq-trading.com/docs/gateways/deribit/index.html)

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-coinbase-pro
```

It is easiest to start from a config file template

```bash
cp $CONDA_PREFIX/share/roq-coinbase-pro/config.toml ~/coinbase-pro.toml
```

Edit this file and update with your Coinbase Pro API credentials
([link](https://public.sandbox.pro.coinbase.com/profile/api)).

You should look for these lines and replace as appropriate

```text
login = "YOUR_COINBASE_PRO_API_KEY_GOES_HERE"
password = "YOUR_COINBASE_PRO_PASSPHRASE_GOES_HERE"
secret = "YOUR_COINBASE_PRO_SECRET_GOES_HERE"
```

Launch the gateway

```bash
roq-coinbase-pro \
    --name "coinbase-pro" \
    --config_file ~/coinbase-pro.toml \
    --client_listen_address ~/coinbase-pro.sock
```

## License

The project is released under the terms of the BSD 3-Clause license.


## Links

* [Roq GmbH](https://roq-trading.com/)
* [Documentation](https://roq-trading.com/docs/)
* [Issues](https://github.com/roq-trading/roq-issues/issues)
* [Contact us](mailto:info@roq-trading.com)
