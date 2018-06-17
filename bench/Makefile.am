TESTS = samples-benchmark
check_PROGRAMS = samples-benchmark
samples_benchmark_LDADD = \
	$(top_builddir)/src/common/libcommon.la \
	$(ROQ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUCL_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)  \
	$(BENCHMARK_LIBS)  \
	-lpthread
samples_benchmark_CFLAGS = -pthread
samples_benchmark_CPPFLAGS = \
	-DROQ_SPDLOG \
	-I$(top_builddir)/src \
	$(ROQ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUCL_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS) \
	$(BENCHMARK_CFLAGS)
nodist_samples_benchmark_SOURCES = \
	main.cpp
