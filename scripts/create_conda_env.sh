#!/usr/bin/env bash

set -e

# Locate existing Conda installation

echo -e "\033[1;34mFinding install directory...\033[0m"

PREFIX="${CONDA_PREFIX:-$HOME/miniconda3}"

if [[ ! -d "$PREFIX" ]]; then
  (>&2 echo -e "\033[1;31mERROR: Unable to find miniconda3.\033[0m") && exit 1
fi

# Install Conda's compiler toolchain

echo -e "\033[1;34mInstalling required packages...\033[0m"

UNAME="$(uname -s)"

case "$UNAME" in
  Linux*)
    PLATFORM_PACKAGES="gcc_linux-64 gxx_linux-64 patchelf"
    ;;
  Darwin*)
    PLATFORM_PACKAGES="clang_osx-64 clangxx_osx-64"
    ;;
  *)
    (>&2 echo -e "\033[1;31mERROR: Unknown platform.\033[0m") && exit 1
    ;;
esac

$PREFIX/bin/conda install --yes \
  --override-channels \
  --channel anaconda \
  --channel https://roq-trading.com/conda/unstable \
  $PLATFORM_PACKAGES \
  autoconf \
  automake \
  libtool \
  pkgconfig \
  make \
  cmake \
  git \
  roq-api

# Use Conda's environment-specific variables

echo -e "\033[1;34mDefining the build environment...\033[0m"

mkdir -p "$PREFIX/etc/conda/activate.d"

cat > "$PREFIX/etc/conda/activate.d/roq-dev.sh" << 'EOF'
export PREFIX="$CONDA_PREFIX"
export CPPFLAGS="$CPPFLAGS -I$PREFIX/include"
export LDFLAGS="$LDFLAGS -L$PREFIX/lib"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"
EOF

if [[ ! -z $CONDA_PREFIX ]]; then
  echo -e "\033[1;33mPlease re-activate your conda environment now!\033[0m"
  echo -e "\033[1;30msource deactivate && source $PREFIX/bin/activate\033[0m"
else
  echo -e "\033[1;33mYou can activate your conda environment now!\033[0m"
  echo -e "\033[1;30msource $PREFIX/bin/activate\033[0m"
fi

# Finished

echo -e "\033[1;31mDone!\033[0m"
