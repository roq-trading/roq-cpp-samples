#!/usr/bin/env bash

[[ "${BASH_SOURCE[0]}" == "${0}" ]] && >&2 echo "Error! You should source this script." && exit 1

[[ -z "${CONDA_PREFIX}" ]] && >&2 echo "Error! You must run this script from within a conda environment." && return 1

if [ "$1" = "install" ]; then

  UNAME="$(uname -s)"

  case "$UNAME" in
    Linux*)
      conda install -y autoconf automake libtool pkgconfig make cmake gcc_linux-64 gxx_linux-64 patchelf openssl git roq-api
      ;;
    Darwin*)
      conda install -y autoconf automake libtool pkgconfig make cmake clang_osx-64 clangxx_osx-64 git roq-api
      ;;
    *)
      ENV_FILE="** UNKNOWN **"
      ;;
  esac

fi

export LDFLAGS="$LDFLAGS -L$CONDA_PREFIX/lib"
export CPPFLAGS="$CPPFLAGS -I$CONDA_PREFIX/include"
export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig"
