TESTS = roq-samples-test
check_PROGRAMS = roq-samples-test

# SO 35998856
check_LTLIBRARIES = libgtest.la
libgtest_la_SOURCES = \
	$(top_srcdir)/third_party/googletest/googletest/src/gtest-all.cc
libgtest_la_CPPFLAGS = \
	-I$(top_srcdir)/third_party/googletest/googletest/include \
	-I$(top_srcdir)/third_party/googletest/googletest
libgtest_la_LDFLAGS = -pthread

roq_samples_test_LDADD = \
	$(top_builddir)/src/utilities/libutilities.la \
	libgtest.la \
	$(CCTZ_LIBS)
roq_samples_test_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/third_party/googletest/googletest/include \
	$(CCTZ_CFLAGS)
nodist_roq_samples_test_SOURCES = \
	test_time_schedule.cpp \
	main.cpp
