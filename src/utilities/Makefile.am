lib_LTLIBRARIES = libutilities.la
libutilities_la_LDFLAGS = \
	-static
libutilities_la_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(ROQ_CFLAGS) \
	$(CCTZ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
libutilities_la_SOURCES = \
	csv_reader.cpp \
	matcher.cpp \
	generator.cpp \
	time_schedule.cpp
