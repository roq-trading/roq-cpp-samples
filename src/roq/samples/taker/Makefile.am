bin_PROGRAMS = roq-samples-taker
roq_samples_taker_LDADD = \
	$(PTHREAD_LIBS) \
	$(GFLAGS_LIBS) \
	$(ROQ_API_LIBS) \
	$(ROQ_CLIENT_LIBS) \
	$(top_builddir)/src/roq/samples/common/libroq-samples-common.la
roq_samples_taker_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(ROQ_API_CFLAGS) \
	$(ROQ_CLIENT_CFLAGS) \
	-I$(top_srcdir)/src
roq_samples_taker_SOURCES = \
	performance.cpp \
	strategy.cpp \
	main.cpp
