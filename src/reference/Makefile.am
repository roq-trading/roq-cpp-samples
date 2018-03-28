bin_PROGRAMS = reference-strategy
reference_strategy_LDFLAGS = \
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
reference_strategy_CPPFLAGS = \
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
reference_strategy_SOURCES = \
  config.cpp \
  config_reader.cpp \
  csv_reader.cpp \
  exposure.cpp \
  gateway_manager.cpp \
  generator.cpp \
  order.cpp \
  order_manager.cpp \
  position_manager.cpp \
  risk_manager.cpp \
  trading_model.cpp \
  utilities.cpp \
  main.cpp
EXTRA_reference_strategy_SOURCES = \
  config.h \
  config_reader.h \
  csv_reader.h \
  exposure.h \
  gateway_manager.h \
  generator.h \
  order.h \
  order_manager.h \
  position_manager.h \
  risk_manager.h \
  trading_model.h \
  utilities.h
configdir=$(datadir)/roq/samples
config_DATA = strategy.conf
