bin_PROGRAMS = roq-samples-collector
roq_samples_collector_LDADD = \
	$(PTHREAD_LIBS) \
	$(GFLAGS_LIBS) \
	$(ROQ_CLIENT_LIBS) \
	$(ROQ_API_LIBS) \
	$(ROQ_CLIENT_LIBS)
roq_samples_collector_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(ROQ_API_CFLAGS) \
	$(ROQ_CLIENT_CFLAGS) \
	-I$(top_srcdir)/src
roq_samples_collector_SOURCES = \
	collector.cpp \
	main.cpp
