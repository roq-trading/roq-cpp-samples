# Roq Trading Solutions

Solutions focused on development, testing and deployment of
algorithmic trading strategies.


## Overview

The samples are here to help you get started with development and
testing.

Topics

* Creating a development environment.
* Application design.
* How to manage asynchronous events.
* Typical strategy patterns.


## First Steps

### Install Miniconda

> *This section will demonstrate how to install Miniconda*.

A Conda environment allows you to install up-to-date binary packages
on your development box without requiring root access.

```bash
# download the miniconda installer
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

# install miniconda (to your home directory)
bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

# define conda channels
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

The following sections will assume you have already activated your
Conda environment.


## Development

### Prepare the Environment

This script will

* Install the compiler toolchain
* Install Roq dependency packages
* Configure the Conda environment

```bash
./create_conda_env.sh
```

You will now be prompted to start or restart Conda.
Just follow the instructions written to your terminal.

### Update git submodules

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
