bin_PROGRAMS = roq-samples-collector
roq_samples_collector_LDFLAGS = \
	$(top_builddir)/src/utilities/libutilities.la \
	$(ROQ_LIBS) \
	$(CCTZ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
roq_samples_collector_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CCTZ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
roq_samples_collector_SOURCES = \
	collector.cpp \
	main.cpp
EXTRA_roq_samples_collector_SOURCES = \
	collector.h
