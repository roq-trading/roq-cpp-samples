TESTS = roq-samples-test
check_PROGRAMS = roq-samples-test
roq_samples_test_LDADD = \
	$(top_builddir)/src/utilities/libutilities.la \
	$(GTEST_LIBS) \
	$(CCTZ_LIBS)
roq_samples_test_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(GTEST_CFLAGS) \
	$(CCTZ_CFLAGS)
nodist_roq_samples_test_SOURCES = \
	test_time_schedule.cpp
