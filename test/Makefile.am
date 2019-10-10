# SO 35998856
check_LTLIBRARIES = libgtest.la
libgtest_la_SOURCES = \
	$(GTEST_DIR)/googletest/src/gtest-all.cc
libgtest_la_CPPFLAGS = \
	$(GTEST_CFLAGS)
libgtest_la_LDFLAGS = \
	$(PTHREAD_CFLAGS)

TESTS = roq-samples-test
check_PROGRAMS = roq-samples-test
roq_samples_test_LDADD = \
	$(PTHREAD_CFLAGS) \
	libgtest.la
roq_samples_test_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(GTEST_CFLAGS) \
	-I$(top_srcdir)/src
nodist_roq_samples_test_SOURCES = \
	main.cpp
