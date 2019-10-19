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
 src/roq/Makefile
 src/roq/samples/Makefile
 src/roq/samples/example-1/Makefile
 src/roq/samples/example-2/Makefile
 src/roq/samples/example-3/Makefile
 test/Makefile
])
AC_CONFIG_FILES([roq-samples.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
LT_INIT([disable-shared])
AC_PROG_CXX
AC_LANG([C++])

AX_PTHREAD

ROQ_CHECK_STDCXX

ROQ_CHECK_FMT
ROQ_CHECK_GFLAGS

PKG_CHECK_MODULES([ROQ_API], [roq-api])
PKG_CHECK_MODULES([ROQ_CLIENT], [roq-client])

ROQ_CHECK_GTEST([third_party/googletest])

AC_ARG_ENABLE([doc],
  [AS_HELP_STRING([--enable-doc], [Enable documentation])],,
  [enable_doc=no])

AS_IF([test "x$enable_doc" = "xyes"], [
  AC_CHECK_PROG(HAVE_DOXYGEN, [doxygen], [yes], [no])
  ],[
  HAVE_DOXYGEN=no
  ])

AM_CONDITIONAL([ENABLE_DOC],
  [test "x$enable_doc" = "xyes"])

AS_IF([test "x$enable_doc" = "xyes"],
      [AC_CONFIG_FILES([doc/Makefile doc/Doxyfile])])

AC_ARG_ENABLE([benchmark],
	      AS_HELP_STRING([--enable-benchmark], [Enable benchmark]),
	      [],
	      [enable_urls=no])

AS_IF([test "x$enable_benchmark" = "xyes"],
      [AC_CONFIG_FILES([benchmark/Makefile])])

AS_IF(
  [test "x$enable_benchmark" = "xyes"],
  [ROQ_CHECK_BENCHMARK])

AM_CONDITIONAL([ENABLE_BENCHMARK],
	       [test "$enable_benchmark" = "yes"])

AC_OUTPUT
