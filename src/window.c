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

/**
 * Load our built-in CSS resource
 */
static void init_styles(void)
{
        GtkCssProvider *css_provider;
        GFile *file = NULL;
        GdkScreen *screen;

        screen = gdk_screen_get_default();

        /* Fallback */
        css_provider = gtk_css_provider_new();
        file = g_file_new_for_uri("resource://com/github/ikeydoherty/columbiad/style.css");
        if (gtk_css_provider_load_from_file(css_provider, file, NULL)) {
                gtk_style_context_add_provider_for_screen(screen,
                        GTK_STYLE_PROVIDER(css_provider),
                        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        }
        g_object_unref(css_provider);
        g_object_unref(file);
}

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
        init_styles();
        gtk_window_set_title(GTK_WINDOW(self), "Columbiad - Fullscreen in Future!");
        gtk_window_set_position(GTK_WINDOW(self), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(self), 800, 600);
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
