bin_PROGRAMS = roq-samples-simple
roq_samples_simple_LDADD = \
	$(PTHREAD_LIBS) \
	$(GFLAGS_LIBS) \
	$(ROQ_API_LIBS) \
	$(ROQ_CLIENT_LIBS)
roq_samples_simple_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(ROQ_API_CFLAGS) \
	$(ROQ_CLIENT_CFLAGS) \
	-I$(top_srcdir)/src
roq_samples_simple_SOURCES = \
	config.cpp \
	strategy.cpp \
	main.cpp
