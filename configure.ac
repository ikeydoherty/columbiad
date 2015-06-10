AC_INIT([columbiad], 1, [ikey.doherty@gmail.com], [columbiad], [https://github.com/ikeydoherty/columbiad/])
AM_INIT_AUTOMAKE([-Wno-portability no-dist-gzip dist-xz foreign subdir-objects])
AC_PROG_CC
AC_PROG_CC_STDC
LT_PREREQ(2.2)
AC_CONFIG_HEADERS([config.h])
AC_PREFIX_DEFAULT(/usr/local)
AM_SILENT_RULES([yes])
LT_INIT([disable-static])


# Package requirements
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0 >= 2.40.0])
PKG_CHECK_MODULES([GIO], [gio-2.0 >= 2.40.0])
PKG_CHECK_MODULES([GTK3], [gtk+-3.0 >= 3.14.0])

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AC_CONFIG_MACRO_DIR([m4])

# For various Vala things.
PKG_CHECK_MODULES([GEE], [gee-0.8])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AC_MSG_RESULT([
        columbiad $VERSION
        ========

        prefix:                 ${prefix}
        sysconfdir:             ${sysconfdir}
        exec_prefix:            ${exec_prefix}
        bindir:                 ${bindir}

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}
])
