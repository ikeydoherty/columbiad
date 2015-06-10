/*
 * window.c
 * 
 * Copyright 2015 Ikey Doherty <ikey.doherty@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "window.h"

G_DEFINE_TYPE(MainWindow, main_window, GTK_TYPE_APPLICATION_WINDOW)

/* Boilerplate GObject code */
static void main_window_class_init(MainWindowClass *klass);
static void main_window_init(MainWindow *self);
static void main_window_dispose(GObject *object);

/* Initialisation */
static void main_window_class_init(MainWindowClass *klass)
{
        GObjectClass *g_object_class;

        g_object_class = G_OBJECT_CLASS(klass);
        g_object_class->dispose = &main_window_dispose;
}

/**
 * Get the ball rolling
 */
static void main_window_init(MainWindow *self)
{
        gtk_widget_show_all(GTK_WIDGET(self));
}

static void main_window_dispose(GObject *object)
{
        G_OBJECT_CLASS (main_window_parent_class)->dispose (object);
}

/* Utility; return a new MainWindow */
MainWindow *main_window_new(GApplication *application)
{
        MainWindow *self;

        self = g_object_new(MAIN_WINDOW_TYPE, "application", application, NULL);
        return self;
}
