TESTS = roq-samples-benchmark
bin_PROGRAMS = roq-samples-benchmark
roq_samples_benchmark_LDADD = \
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
roq_samples_benchmark_CFLAGS = -pthread
roq_samples_benchmark_CPPFLAGS = \
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
nodist_roq_samples_benchmark_SOURCES = \
	main.cpp
