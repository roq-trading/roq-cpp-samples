#!/usr/bin/env bash

[[ "${BASH_SOURCE[0]}" == "${0}" ]] && >&2 echo "Error! You should source this script." && exit 1

[[ -z "${CONDA_PREFIX}" ]] && >&2 echo "Error! You must run this script from within a conda environment." && return 1

if [ "$1" = "install" ]; then
	conda install -y \
		gcc_linux-64 \
		pkgconfig \
		cmake \
		autoconf automake libtool \
		git \
		roq-api
fi

export LDFLAGS="$LDFLAGS -L$CONDA_PREFIX/lib"
export CPPFLAGS="$CPPFLAGS -I$CONDA_PREFIX/include"
export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"
