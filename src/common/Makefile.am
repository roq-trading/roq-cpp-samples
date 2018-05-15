lib_LTLIBRARIES = libcommon.la
libcommon_la_CPPFLAGS = \
	-DROQ_SPDLOG \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CCTZ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
libcommon_la_SOURCES = \
	account.cpp \
	base_strategy.cpp \
	config.cpp \
	config_variables.cpp \
	gateway.cpp \
	instrument.cpp \
	position.cpp
libcommon_la_includedir = $(includedir)/roq/samples
libcommon_la_include_HEADERS = \
	account.h \
	base_strategy.h \
	config.h \
	config_variables.h \
	gateway.h \
	instrument.h \
	market_data.h \
	position.h
