bin_PROGRAMS = example-collector
example_collector_LDFLAGS = \
	$(QUINCLAS_TRADINGAPI_LIBS) \
	$(CCTZ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
example_collector_CPPFLAGS = \
	-DQUINCLAS_SPDLOG \
	-I$(top_srcdir)/src \
	$(QUINCLAS_TRADINGAPI_CFLAGS) \
	$(CCTZ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
example_collector_SOURCES = \
	collector.cpp \
	csv_reader.cpp \
	generator.cpp \
	main.cpp
EXTRA_example_collector_SOURCES = \
	collector.h \
	csv_reader.h \
	generator.h
