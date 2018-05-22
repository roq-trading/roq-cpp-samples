AC_INIT([roq-samples],
        [0.1.5],
        [thraneh@gmail.com],
        [roq-samples])

AC_PREREQ([2.59])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/utilities/Makefile
 src/common/Makefile
 src/collector/Makefile
 src/simple/Makefile
 src/spread/Makefile
 src/config/Makefile
])
AC_CONFIG_FILES([roq-samples.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

AC_DISABLE_SHARED

ROQ_CHECK_LIBUNWIND
ROQ_CHECK_CROSSGUID
ROQ_CHECK_SPDLOG
ROQ_CHECK_GFLAGS
ROQ_CHECK_DOUBLE_CONVERSION
ROQ_CHECK_CCTZ
ROQ_CHECK_FLATBUFFERS
ROQ_CHECK_LIBEVENT
ROQ_CHECK_LIBUCL
ROQ_CHECK_RAPIDJSON

PKG_CHECK_MODULES([ROQ], [roq])

AC_OUTPUT
