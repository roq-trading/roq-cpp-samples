m4_include(m4/ax_cxx_compile_stdcxx.m4)  # https://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx.html

AC_INIT([quinclas-examples],
        [0.1],
        [thraneh@gmail.com],
        [quinclas-examples])

AC_PREREQ([2.59])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/collector/Makefile
 src/reference/Makefile
])
AC_CONFIG_FILES([quinclas-examples.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# Check required programs.
AM_PROG_AR
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

# Check C++ 11 support.
AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

AC_DISABLE_SHARED

# Find libunwind.
PKG_CHECK_MODULES([LIBUNWIND], [libunwind], [], [
  AC_CHECK_HEADER(
    [libunwind.h],
    [AC_SUBST([LIBUNWIND_CFLAGS])],
    [AC_MSG_ERROR([unable to find libunwind header files])]
  )
  case $host_os in
  darwin* )
    AC_MSG_NOTICE([No library needed on Darwin])
    AC_SUBST([LIBUNWIND_LIBS], [])
    ;;
  * )
    AC_SUBST([LIBUNWIND_LIBS], [-lunwind])
    ;;
  esac
])

# Find crossguid.
PKG_CHECK_MODULES([CROSSGUID], [crossguid], [], [
  AC_CHECK_HEADER(
    [crossguid/guid.hpp],
    [
      AC_SUBST([CROSSGUID_CFLAGS])
      AC_DEFINE([HAVE_CROSSGUID], 1)
      case $host_os in
      darwin* )
        AC_SUBST([CROSSGUID_LIBS], ['-lcrossguid -framework CoreFoundation'])
        ;;
      * )
        AC_SUBST([CROSSGUID_LIBS], ['-lcrossguid -luuid'])
        ;;
      esac
    ],
    [
     AC_MSG_WARN([unable to find crossguid header files])
     AC_SUBST([CROSSGUID_LIBS], [])
    ]
  )
])

# Find spdlog.
PKG_CHECK_MODULES([SPDLOG], [spdlog], [], [
  AC_CHECK_HEADER(
    [spdlog/spdlog.h],
    [AC_SUBST([SPDLOG_CFLAGS])],
    [AC_MSG_ERROR([unable to find spdlog header files])]
  )
  AC_SUBST([SPDLOG_LIBS], [])
])

# Find gflags.
PKG_CHECK_MODULES([GFLAGS], [gflags], [], [
  AC_CHECK_HEADER(
    [gflags/gflags.h],
    [AC_SUBST([GFLAGS_CFLAGS])],
    [AC_MSG_ERROR([unable to find gflags header files])]
  )
  AC_SUBST([GFLAGS_LIBS], [-lgflags])
])

# Find double-conversion.
PKG_CHECK_MODULES([DOUBLE_CONVERSION], [double-conversion], [], [
  AC_CHECK_HEADER(
    [double-conversion/double-conversion.h],
    [AC_SUBST([DOUBLE_CONVERSION_CFLAGS])],
    [AC_MSG_ERROR([unable to find double-conversion header files])]
  )
  AC_SUBST([DOUBLE_CONVERSION_LIBS], [-ldouble-conversion])
])

# Find cctz.
PKG_CHECK_MODULES([CCTZ], [cctz], [], [
  AC_CHECK_HEADER(
    [cctz/civil_time.h],
    [AC_SUBST([CCTZ_CFLAGS])],
    [AC_MSG_ERROR([unable to find cctz header files])]
  )
  AC_SUBST([CCTZ_LIBS], [-lcctz])
])

# Find flatbuffers.
PKG_CHECK_MODULES([FLATBUFFERS], [flatbuffers], [], [
  AC_CHECK_HEADER(
    [flatbuffers/flatbuffers.h],
    [AC_SUBST([FLATBUFFERS_CFLAGS])],
    [AC_MSG_ERROR([unable to find flatbuffers header files])]
  )
  AC_SUBST([FLATBUFFERS_LIBS], [-lflatbuffers])
])
AC_PATH_PROGS([FLATC], [flatc], [])

# Find libevent.
PKG_CHECK_MODULES([LIBEVENT], [libevent], [], [
  AC_CHECK_HEADER(
    [event2/event.h],
    [AC_SUBST([LIBEVENT_CFLAGS])],
    [AC_MSG_ERROR([unable to find libevent header files])]
  )
  AC_SUBST([LIBEVENT_LIBS], [-levent])
])

# Find ucl.
PKG_CHECK_MODULES([UCL], [libucl], [], [
  AC_CHECK_HEADER(
    [ucl.h],
    [AC_SUBST([UCL_CFLAGS])],
    [AC_MSG_ERROR([unable to find ucl header files])]
  )
  AC_SUBST([UCL_LIBS], [-lucl])
])

# Find tradingapi.
PKG_CHECK_MODULES([QUINCLAS_TRADINGAPI], [quinclas-tradingapi])

AC_OUTPUT
