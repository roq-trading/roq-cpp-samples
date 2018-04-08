bin_PROGRAMS = example-order-management
example_order_management_LDFLAGS = \
	$(top_builddir)/src/utilities/libutilities.la \
	$(ROQ_LIBS) \
	$(CCTZ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
example_order_management_CPPFLAGS = \
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
example_order_management_SOURCES = \
	base.cpp \
	strategy.cpp \
	main.cpp
EXTRA_example_order_management_SOURCES = \
	base.h \
	market_data.h \
	strategy.h
