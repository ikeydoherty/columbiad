EXTRA_DIST = \
	README.md \
	LICENSE \
	data/style.css \
	data/app.gresource.xml

AM_CFLAGS = -fstack-protector -Wall -Wno-pedantic \
        -Wstrict-prototypes -Wundef -fno-common \
        -Werror-implicit-function-declaration \
        -Wformat -Wformat-security -Werror=format-security \
        -Wno-conversion -Wunreachable-code -std=c99 -Werror

AM_CPPFLAGS = \
	-I $(top_srcdir) \
	-I $(top_srcdir)/src

app_resources = $(shell glib-compile-resources --sourcedir=$(top_srcdir)/data --generate-dependencies $(top_srcdir)/data/app.gresource.xml)

BUILT_SOURCES = \
	app-resources.h \
	app-resources.c

CLEANFILES = \
	$(BUILT_SOURCES)

# resources
app-resources.h: $(top_srcdir)/data/app.gresource.xml $(app_resources)
	glib-compile-resources --target=$@ --sourcedir=$(top_srcdir)/data --generate-header --c-name app $<

app-resources.c: $(top_srcdir)/data/app.gresource.xml $(app_resources)
	glib-compile-resources --target=$@ --sourcedir=$(top_srcdir)/data --generate-source --c-name app $<
	

bin_PROGRAMS = columbiad

columbiad_SOURCES = \
	src/main.c \
	src/window.h \
	src/window.c \
	app-resources.h \
	app-resources.c

columbiad_CFLAGS = \
	$(GOBJECT_CFLAGS) \
	$(GTK3_CFLAGS) \
	$(AM_CFLAGS)

columbiad_LDADD = \
	$(GOBJECT_LIBS) \
	$(GTK3_LIBS)
