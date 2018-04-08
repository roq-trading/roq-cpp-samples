Copyright (c) 2017-2018, Hans Erik Thrane

## Usage

If you build and run

	./example-simple --config-file simple.conf --config-variables instruments.conf --gateways femasapi=test:1234@/var/tmp/femasapi.sock

If you use the Conda package

	example-simple --config-file $CONDA_PREFIX/share/roq/samples simple.conf --config-variables $CONDA_PREFIX/share/roq/samples/instruments.conf --gateways femasapi=test:1234@/var/tmp/femasapi.sock
