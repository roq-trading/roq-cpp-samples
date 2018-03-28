# Examples demonstrating how to use the trading interface.

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

## Overview

This repository contains various examples using the trading interface.

## Ubuntu (Not Required!)

**We recommend using Conda's build environment!
This is only necessary if you want to use Ubuntu's native build environment.**

	sudo apt-get install -y autoconf autoconf-archive libtool pkgconfig gcc gdb

## Using Conda

Download Miniconda [here](https://conda.io/miniconda.html).

	wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

Create the Conda environment.

	# install miniconda
	bash Miniconda3-latest-Linux-x86_64.sh -b

	# activate miniconda
	source miniconda3/bin/activate

	# define conda channels
	cat > miniconda3/.condarc << EOF
	channels:
	  - anaconda
	  - http://roq-trading.com/dist/conda/unstable
	EOF

Install the dependencies (Conda packages).

	# install the trading api
	conda install -y roq

	# THIS STEP IS OPTIONAL (see comment under the section about Ubuntu)
	# install the build toolchain (so we can build from source)
	conda install -y autoconf automake libtool pkgconfig gcc_linux-64

	# we also need git so we can clone from github
	conda install -y git

Compile the project.

	# environment variables allowing the build tool to find dependencies
	export LDFLAGS=-L"$CONDA_PREFIX/lib"
	export CPPFLAGS=-I"$CONDA_PREFIX/include"
	export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"

	# clone from github
	git clone https://github.com/roq-trading/roq-samples
	cd roq-samples

	# configure the project
	./autogen.sh && ./configure --prefix "$CONDA_PREFIX"

	# build
	make -j4

	# test it works
	cd src/collector
	./example-collector --mode simulation --simulation-file SOME_FILE

Here, for simplicity, we simply install into the Conda root environment.
Best practice is to create a specific environment and *not* simply use the root environment.
Please refer to the Conda documentation ([link](https://conda.io/docs/user-guide/tasks/manage-environments.html)) for further details on how to create environments.
