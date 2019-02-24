Copyright (c) 2017-2019, Hans Erik Thrane

## Simulation

If you build from source

	./roq-samples-simple --config-file simple.conf --simulation-file USTP_Multi_20180223.csv



## Live

If you build from source

	./roq-samples-simple --config-file simple.conf --gateways femasapi=test:1234@/var/tmp/femasapi.sock

If you use the Conda package

	roq-samples-simple --config-file $CONDA_PREFIX/share/roq/samples simple.conf --gateways femasapi=test:1234@/var/tmp/femasapi.sock
