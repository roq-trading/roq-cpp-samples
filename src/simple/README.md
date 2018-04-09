Copyright (c) 2017-2018, Hans Erik Thrane

## Simulation

If you build from source

	./example-simple --config-file simple.conf --config-variables instruments.conf --simulation-file USTP_Multi_20180223.csv



## Live

If you build from source

	./example-simple --config-file simple.conf --config-variables instruments.conf --gateways femasapi=test:1234@/var/tmp/femasapi.sock

If you use the Conda package

	example-simple --config-file $CONDA_PREFIX/share/roq/samples simple.conf --config-variables $CONDA_PREFIX/share/roq/samples/instruments.conf --gateways femasapi=test:1234@/var/tmp/femasapi.sock
