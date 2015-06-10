/*
 * common.h
 * 
 * Copyright 2015 Ikey Doherty <ikey.doherty@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 */

#pragma once

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 3 && GTK_MINOR_VERSION >= 12
        #define MARGIN_END "margin-end"
        #define MARGIN_START "margin-start"
#else
        #define MARGIN_END "margin-right"
        #define MARGIN_START "margin-left"
#endif


/* Utility: Add class to a widgets stylecontext */
#define WCLASS(w,c) { gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(w)), c); }
