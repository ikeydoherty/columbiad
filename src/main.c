/*
 * main.c - This file is part of columbiad
 *
 * Copyright (C) 2015 Ikey Doherty <ikey.doherty@gmail.com>
 *
 * fullscreen-tv-launcher is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <gtk/gtk.h>
#include <stdlib.h>

#include "window.h"

static MainWindow *kapp = NULL;

static void app_activate(GApplication *app, __attribute__((unused)) gpointer udata)
{
        if (kapp) {
                goto present;
                return;
        }
        kapp = main_window_new(GTK_APPLICATION(app));
        main_window_init_ui(kapp);
        gtk_widget_show(GTK_WIDGET(kapp));
present:
        gtk_window_present(GTK_WINDOW(kapp));
}

int main(int argc, char **argv)
{
        GtkApplication *app = NULL;
        int status;

        app = gtk_application_new("com.github.ikeydoherty.columbiad", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(G_OBJECT(app), "activate", G_CALLBACK(app_activate), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);

        g_object_unref(app);
        return status;
}
