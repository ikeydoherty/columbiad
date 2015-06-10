EXTRA_DIST = \
	README.md \
	LICENSE

AM_CFLAGS = -fstack-protector -Wall -Wno-pedantic \
        -Wstrict-prototypes -Wundef -fno-common \
        -Werror-implicit-function-declaration \
        -Wformat -Wformat-security -Werror=format-security \
        -Wno-conversion -Wunreachable-code -std=c99 -Werror

AM_CPPFLAGS = \
	-I $(top_srcdir) \
	-I $(top_srcdir)/src

CLEANFILES = \
	$(BUILT_SOURCES)


bin_PROGRAMS = columbiad

columbiad_SOURCES = \
	src/main.c

columbiad_CFLAGS = \
	$(GOBJECT_CFLAGS) \
	$(GTK3_CFLAGS) \
	$(AM_CFLAGS)

columbiad_LDADD = \
	$(GOBJECT_LIBS) \
	$(GTK3_LIBS)
