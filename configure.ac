AC_REVISION([m4_esyscmd_s([build-aux/git-version])])

AC_INIT([roq-samples],
        m4_esyscmd_s([build-aux/git-version]),
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
 test/Makefile
 doc/Makefile
 doc/Doxyfile
])
AC_CONFIG_FILES([roq-samples.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

ROQ_CHECK_STDCXX

AC_DISABLE_SHARED

ROQ_CHECK_LIBUNWIND
ROQ_CHECK_CROSSGUID
ROQ_CHECK_SPDLOG
ROQ_CHECK_GFLAGS
ROQ_CHECK_CCTZ
ROQ_CHECK_FLATBUFFERS
ROQ_CHECK_LIBEVENT
ROQ_CHECK_LIBUCL

PKG_CHECK_MODULES([ROQ], [roq])

ROQ_CHECK_GTEST([third_party/googletest])

AC_ARG_ENABLE([benchmark],
	      AS_HELP_STRING([--enable-benchmark], [Enable benchmark]),
	      [],
	      [enable_benchmark=no])

AS_IF([test "x$enable_benchmark" = "xyes"],
      [AC_CONFIG_FILES([src/benchmark/Makefile])])

AS_IF(
  [test "x$enable_benchmark" = "xyes"],
  [ROQ_CHECK_BENCHMARK])

AM_CONDITIONAL([ENABLE_BENCHMARK],
	       [test "$enable_benchmark" = "yes"])

AC_ARG_ENABLE([doc],
  [AS_HELP_STRING([--disable-doc], [Do not build API documentation])],,
  [enable_doc=yes])

AS_IF([test "$enable_doc" = "yes"], [
  AC_CHECK_PROG(HAVE_DOXYGEN, [doxygen], [yes], [no])
  AC_CHECK_PROG(HAVE_DOT, [dot], [yes], [no])
  ],[
  HAVE_DOXYGEN=no
  ])

AM_CONDITIONAL([HAVE_DOXYGEN], [test "$HAVE_DOXYGEN" = "yes"])

AC_OUTPUT
