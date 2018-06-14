ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . src

if ENABLE_BENCHMARK
SUBDIRS += bench
endif

dist_doc_DATA = LICENSE README.md

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

.PHONY: lint

lint:
	cpplint --linelength=120 --recursive src/ bench/
