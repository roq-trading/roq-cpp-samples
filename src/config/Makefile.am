bin_PROGRAMS = example-config
example_config_LDFLAGS = \
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
example_config_CPPFLAGS = \
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
example_config_SOURCES = \
	json.cpp \
	key_value.cpp \
	main.cpp
