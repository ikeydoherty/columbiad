/*
 * This file is part of columbiad.
 *
 * Copyright © 2015-2017 Ikey Doherty <ikey@solus-project.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#include <stdbool.h>

#include "carousel.h"

typedef struct _MainWindow MainWindow;
typedef struct _MainWindowClass MainWindowClass;

#define MAIN_WINDOW_TYPE (main_window_get_type())
#define MAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MAIN_WINDOW_TYPE, MainWindow))
#define IS_MAIN_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MAIN_WINDOW_TYPE))
#define MAIN_WINDOW_CLASS(klass)                                                                   \
        (G_TYPE_CHECK_CLASS_CAST((klass), MAIN_WINDOW_TYPE, MainWindowClass))
#define IS_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MAIN_WINDOW_TYPE))
#define MAIN_WINDOW_GET_CLASS(obj)                                                                 \
        (G_TYPE_INSTANCE_GET_CLASS((obj), MAIN_WINDOW_TYPE, MainWindowClass))

/* MainWindow object */
struct _MainWindow {
        GtkApplicationWindow parent;
        GtkWidget *clock;
        guint clock_id;
        GMenu *menu;
        GtkWidget *button;
        GtkWidget *apps;
        bool fullscreen;
};

/* MainWindow class definition */
struct _MainWindowClass {
        GtkApplicationWindowClass parent_class;
};

GType main_window_get_type(void);

/* MainWindow methods */

/**
 * Finish initialisation, i.e. actions, etc
 * */
void main_window_init_ui(MainWindow *self);

/**
 * Construct a new MainWindow
 *
 * @param app Parent application instance
 * @return A new MainWindow
 */
MainWindow *main_window_new(GtkApplication *application);
