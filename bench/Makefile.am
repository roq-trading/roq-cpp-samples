TESTS = samples-benchmark
check_PROGRAMS = samples-benchmark
samples_benchmark_LDADD = \
	$(top_builddir)/src/common/libcommon.la \
	$(BENCHMARK_LIBS)  \
	-lpthread
samples_benchmark_CFLAGS = -pthread
samples_benchmark_CPPFLAGS = \
	-I$(top_builddir)/src \
	$(BENCHMARK_CFLAGS)
nodist_samples_benchmark_SOURCES = \
	main.cpp
