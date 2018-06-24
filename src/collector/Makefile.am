bin_PROGRAMS = example-collector
example_collector_LDFLAGS = \
	$(top_builddir)/src/utilities/libutilities.la \
	$(ROQ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
example_collector_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
example_collector_SOURCES = \
	collector.cpp \
	main.cpp
EXTRA_example_collector_SOURCES = \
	collector.h
