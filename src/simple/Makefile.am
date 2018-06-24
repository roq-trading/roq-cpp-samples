bin_PROGRAMS = example-simple
example_simple_LDFLAGS = \
	$(top_builddir)/src/utilities/libutilities.la \
	$(top_builddir)/src/common/libcommon.la \
	$(ROQ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUCL_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
example_simple_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUCL_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
example_simple_SOURCES = \
	config.cpp \
	config_reader.cpp \
	strategy.cpp \
	main.cpp
EXTRA_example_simple_SOURCES = \
	config.h \
	config_reader.h \
	strategy.h
configdir=$(datadir)/roq/samples
config_DATA = simple.conf
