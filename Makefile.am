ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . src test

dist_doc_DATA = LICENSE README.md

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

check-local:
	cd ${GTEST_DIR} && $(MAKE) $(AM_MAKEFLAGS) lib/libgtest.la lib/libgtest_main.la

.PHONY: lint

lint:
	cpplint --linelength=120 --recursive src/
