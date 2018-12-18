#!/usr/bin/env bash

set -e

env

export CPPFLAGS="-I$PREFIX/include -DNDEBUG $CPPFLAGS"
export LDFLAGS="-L$PREFIX/lib $LDFLAGS"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

./autogen.sh
./configure \
  --prefix=$PREFIX \
  --enable-benchmark \
  --enable-doc
make -j${CPU_COUNT:-2}
## FIXME(thraneh): make check doesn't work on Darwin
#if [[ $(uname -a) =~ "Linux" ]]; then
#make check
#fi
make install
