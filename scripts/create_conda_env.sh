#!/usr/bin/env bash

if [[ ! -z $CONDA_PREFIX ]]; then
  (>&2 echo -e "\033[1;31mERROR: Please deactivate your conda environment before using this script. Use 'conda deactivate'.\033[0m") && exit 1
fi

OPT_DIR="./opt"
CONDA_DIR="$OPT_DIR/conda"

if [[ -d "$CONDA_DIR" ]]; then
  (>&2 echo -e "\033[1;31mERROR: Refusing to overwrite an existing conda installation. Directory is '$CONDA_DIR'.\033[0m") && exit 1
fi

if [[ -z $1 ]]; then
  (>&2 echo -e "\033[1;31mERROR: Expected first argument to be a build. Use 'stable' or 'unstable'..\033[0m") && exit 1
fi

BUILD="$1"

echo "BUILD=$BUILD"

case "$BUILD" in
  stable)
    ;;
  unstable)
    ;;
  *)
    (>&2 echo -e "\033[1;31mERROR: Unknown build. Should be 'stable' or 'unstable'.\033[0m") && exit 1
    ;;
esac

if [[ -z $2 ]]; then
  (>&2 echo -e "\033[1;31mERROR: Expected second argument to be a target. Use 'debug' or 'release'..\033[0m") && exit 1
fi

TARGET="$2"

echo "TARGET=$TARGET"

case "$TARGET" in
  debug)
    ;;
  release)
    ;;
  *)
    (>&2 echo -e "\033[1;31mERROR: Unknown target. Should be 'debug' or 'release'.\033[0m") && exit 1
    ;;
esac

ARCH="$(uname -m)"

case "$ARCH" in
  x86_64)
    CONDA_ARCH="x86_64"
    CONDA_PKG_EXT="64"
    ;;
  aarch64)
    CONDA_ARCH="aarch64"
    CONDA_PKG_EXT="aarch64"
    ;;
  arm64)
    CONDA_ARCH="arm64"
    CONDA_PKG_EXT="arm64"
    ;;
  *)
    (>&2 echo -e "\033[1;31mERROR: Unknown architecture.\033[0m") && exit 1
esac

echo "CONDA_ARCH=$CONDA_ARCH"
echo "CONDA_PKG_EXT=$CONDA_PKG_EXT"

KERNEL="$(uname -s)"

case "$KERNEL" in
  Linux*)
    CONDA_OS="Linux"
    ;;
  Darwin*)
    CONDA_OS="MacOSX"
    ;;
  *)
    (>&2 echo -e "\033[1;31mERROR: Unknown kernel.\033[0m") && exit 1
    ;;
esac

echo "CONDA_OS=$CONDA_OS"

CONDA_INSTALLER="Mambaforge-$CONDA_OS-$CONDA_ARCH.sh"

echo "CONDA_INSTALLER=$CONDA_INSTALLER"

mkdir -p "$OPT_DIR"

if [[ ! -f "$OPT_DIR/$CONDA_INSTALLER" ]]; then

  echo -e "\033[1;34mDownload installer...\033[0m"

  MINIFORGE_URL="${MINIFORGE_URL:-https://github.com/conda-forge/miniforge/releases/latest/download}"
  CONDA_DOWNLOAD_URL="$MINIFORGE_URL/$CONDA_INSTALLER"

  curl $CURL_PARAMS \
    --location \
    --output "$OPT_DIR/$CONDA_INSTALLER" \
    "$CONDA_DOWNLOAD_URL"

else

  echo -e "\033[1;34mUsing existing installer.\033[0m"

fi

echo -e "\033[1;34mInstall conda...\033[0m"

bash "$OPT_DIR/$CONDA_INSTALLER" -b -p "$CONDA_DIR"

# note! mamba: default timeout seems too short
export MAMBA_NO_LOW_SPEED_LIMIT=1

echo -e "\033[1;34mInstall compiler...\033[0m"

case "$KERNEL" in
  Linux*)
    "$CONDA_DIR/bin/mamba" install -y "gxx_linux-$CONDA_PKG_EXT>=13"
    ;;
  Darwin*)
    "$CONDA_DIR/bin/mamba" install -y "clang_osx-$CONDA_PKG_EXT>=16"
    ;;
esac

echo -e "\033[1;34mInstall toolchain...\033[0m"

"$CONDA_DIR/bin/mamba" install -y \
  'clangdev>=16' \
  'cmake>=3.25' \
  conda-build \
  make \
  pkg-config

echo -e "\033[1;34mInstall dependencies...\033[0m"

"$CONDA_DIR/bin/mamba" install -y \
  benchmark \
  'catch2>=3.3' \
  jinja2 \
  libevent \
  nlohmann_json \
  pybind11 \
  tomlplusplus

echo -e "\033[1;34mInstall dependencies from $BUILD...\033[0m"

"$CONDA_DIR/bin/mamba" install -y --channel "https://roq-trading.com/conda/$BUILD" \
  roq-client \
  roq-tools

echo -e "\033[1;34mInstall conda activation script...\033[0m"

CONDA_ACTIVATION_DIR="$CONDA_DIR/etc/conda/activate.d"

mkdir -p "$CONDA_ACTIVATION_DIR"

CONDA_ACTIVATION_SCRIPT="$CONDA_ACTIVATION_DIR/roq.sh"

# note! copying CPPFLAGS to CXXFLAGS because cmake doesn't use CPPFLAGS

if [[ "$KERNEL" =~ .*Linux.* ]]; then
  case "$TARGET" in
    release)
      cat > "$CONDA_ACTIVATION_SCRIPT" << 'EOF'
CFLAGS="${CFLAGS/-march=nocona/-march=broadwell}"
CFLAGS="${CFLAGS/-mtune=haswell/-mtune=broadwell}"
CXXFLAGS="${CXXFLAGS/-march=nocona/-march=broadwell}"
CXXFLAGS="${CXXFLAGS/-mtune=haswell/-mtune=broadwell}"
export PREFIX="$CONDA_PREFIX"
export CFLAGS="$CFLAGS -O3"
export CPPFLAGS="$CPPFLAGS -Wall -Wextra -I$PREFIX/include -O3"
export CXXFLAGS="$CXXFLAGS $CPPFLAGS"
export LDFLAGS="$LDFLAGS -L$PREFIX/lib"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"
export ROQ_BUILD_TYPE="release"
EOF
      ;;
    debug)
      cat > "$CONDA_ACTIVATION_SCRIPT" << 'EOF'
export PREFIX="$CONDA_PREFIX"
export CFLAGS="$DEBUG_CFLAGS -fsanitize=address"
export CPPFLAGS="$DEBUG_CPPFLAGS -fsanitize=address -Wall -Wextra -I$PREFIX/include"
export CXXFLAGS="$DEBUG_CXXFLAGS $CPPFLAGS"
export LDFLAGS="$LDFLAGS -L$PREFIX/lib"export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"
export ROQ_BUILD_TYPE="debug"
export ASAN_OPTIONS="strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:detect_leaks=1"
EOF
      ;;
  esac
fi

# note! -Wno-deprecated-builtins due to abseil-cpp and clang 15
if [[ "$KERNEL" =~ .*Darwin.* ]]; then
  case "$TARGET" in
    release)
      cat > "$CONDA_ACTIVATION_SCRIPT" << 'EOF'
export PREFIX="$CONDA_PREFIX"
export CFLAGS="$CFLAGS"
export CPPFLAGS="$CPPFLAGS -Wall -Wextra -Wno-deprecated-builtins -I$PREFIX/include"
export CXXFLAGS="$CXXFLAGS $CPPFLAGS"
export LDFLAGS="$LDFLAGS -L$PREFIX/lib"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"
export ROQ_BUILD_TYPE="release"
EOF
      ;;
    debug)
      cat > "$CONDA_ACTIVATION_SCRIPT" << 'EOF'
export PREFIX="$CONDA_PREFIX"
export CFLAGS="$DEBUG_CFLAGS"
export CPPFLAGS="$DEBUG_CPPFLAGS -Wall -Wextra -Wno-deprecated-builtins -I$PREFIX/include"
export CXXFLAGS="$DEBUG_CXXFLAGS $CPPFLAGS"
export LDFLAGS="$LDFLAGS -L$PREFIX/lib"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"
export ROQ_BUILD_TYPE="debug"
export ASAN_OPTIONS="strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:detect_leaks=1"
EOF
      ;;
  esac
fi

echo -e "\033[1;34mReady!\033[0m"
echo -e "\033[1;34mYou can now activate your conda environment using 'source $CONDA_DIR/bin/activate'.\033[0m"
