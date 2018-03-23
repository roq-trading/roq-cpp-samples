## How To

	# install miniconda
	bash Miniconda3-latest-Linux-x86_64.sh -b

	# activate the root environment
	source miniconda3/bin/activate

	# define conda channels
	cat > miniconda3/.condarc << EOF
	channels:
	  - anaconda
	  - http://quinclas.com/dist/conda/unstable
	EOF

	# install the trading api
	conda install -y quinclas-tradingapi

	# clone the examples repo
	git clone https://github.com/quinclas/examples
	cd examples

	# define environment variables for build tools
	export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"
	export CFLAGS=-I"$CONDA_PREFIX/include"
	export CPPFLAGS="$CFLAGS"
	export LDFLAGS=-L"$CONDA_PREFIX/lib"

	# configure the project
	./autogen.sh && ./configure --prefix="$CONDA_PREFIX"

	# build the project
	make -j4

	# test it works
	cd examples/collector
	./example-collector --mode simulation --simulation-file SOME_FILE
