# Examples demonstrating how to use the trading interface.

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

## Overview

This repository contains various examples using the trading interface.

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
	  - http://quinclas.com/dist/conda/unstable
	EOF

	# install the trading api
	conda install -y quinclas-tradingapi

Compile the examples project.

	# clone examples from github
	git clone https://github.com/quinclas/examples
	cd examples

	# necessary environment variables for the build tool
	export LDFLAGS=-L"$CONDA_PREFIX/lib"
	export CPPFLAGS=-I"$CONDA_PREFIX/include"
	export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"

	# configure the examples project
	./autogen.sh && ./configure --prefix="$CONDA_PREFIX"

	# build examples
	make -j4

	# test it works
	cd examples/collector
	./example-collector --mode simulation --simulation-file SOME_FILE

Here, for simplicity, we simply install into the Conda root environment.
Best practice is to create a specific environment and *not* simply use the root environment.
Please refer to the Conda documentation ([link](https://conda.io/docs/user-guide/tasks/manage-environments.html)) for further details on how to create environments.
