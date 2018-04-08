bin_PROGRAMS = example-simple
example_simple_LDFLAGS = \
	$(top_builddir)/src/utilities/libutilities.la \
	$(ROQ_LIBS) \
	$(CCTZ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUCL_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
example_simple_CPPFLAGS = \
	-DROQ_SPDLOG \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CCTZ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUCL_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
example_simple_SOURCES = \
	base.cpp \
	config.cpp \
	config_reader.cpp \
	strategy.cpp \
	main.cpp
EXTRA_example_simple_SOURCES = \
	base.h \
	config.h \
	config_reader.h \
	market_data.h \
	strategy.h
configdir=$(datadir)/roq/samples
config_DATA = simple.conf instruments.conf
