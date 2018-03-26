#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.66])
AC_INIT(psstop, 1.2, dev@lists.clearlinux.org)
AM_INIT_AUTOMAKE([foreign -Wall -W subdir-objects])
AM_SILENT_RULES([yes])
AC_PROG_CC
AC_LANG(C)
AC_CONFIG_HEADERS([config.h])
PKG_CHECK_MODULES([glib], [glib-2.0 gthread-2.0])
AM_PROG_CC_C_O

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
