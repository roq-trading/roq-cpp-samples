bin_PROGRAMS = roq-samples-benchmark
roq_samples_benchmark_LDADD = \
	$(PTHREAD_LIBS) \
	$(top_builddir)/src/roq/samples/common/libroq-samples-common.la \
	$(ROQ_CLIENT_LIBS) \
	$(ROQ_API_LIBS) \
	$(BENCHMARK_LIBS)
roq_samples_benchmark_CFLAGS = \
	$(PTHREAD_CFLAGS)
roq_samples_benchmark_CPPFLAGS = \
	$(BENCHMARK_CFLAGS) \
	$(ROQ_API_CFLAGS) \
	$(ROQ_CLIENT_CFLAGS) \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/benchmark
nodist_roq_samples_benchmark_SOURCES = \
	strategy.cpp \
	main.cpp
