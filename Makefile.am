EXTRA_DIST = COPYING psstop.spec psstop.1.md

bin_PROGRAMS = psstop

psstop_SOURCES = src/main.c


AM_CPPFLAGS = $(AM_CFLAGS) $(glib_CFLAGS)
psstop_LDADD = $(glib_LIBS)

psstop.1: psstop.1.md
	ronn -r $< --pipe > $@

dist_man_MANS = \
	psstop.1
