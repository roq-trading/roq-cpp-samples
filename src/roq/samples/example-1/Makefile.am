bin_PROGRAMS = roq-samples-example-1
roq_samples_example_1_LDADD = \
	$(PTHREAD_LIBS) \
	$(GFLAGS_LIBS) \
	$(FMT_LIBS) \
	$(ROQ_API_LIBS) \
	$(ROQ_CLIENT_LIBS)
roq_samples_example_1_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(FMT_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(ROQ_API_CFLAGS) \
	$(ROQ_CLIENT_CFLAGS) \
	-I$(top_srcdir)/src
roq_samples_example_1_SOURCES = \
	main.cpp
