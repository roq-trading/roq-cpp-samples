bin_PROGRAMS = roq-samples-spread
roq_samples_spread_LDFLAGS = \
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
roq_samples_spread_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUCL_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
roq_samples_spread_SOURCES = \
	config.cpp \
	config_reader.cpp \
	strategy.cpp \
	main.cpp
EXTRA_roq_samples_spread_SOURCES = \
	config.h \
	config_reader.h \
	strategy.h
configdir=$(datadir)/roq/sampless
config_DATA = spread.conf
