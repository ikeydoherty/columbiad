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

gboolean update_clock(MainWindow *self)
{
        time_t rtime;
        char ftime[6];
        struct tm *ti = NULL;

        time(&rtime);
        ti = localtime(&rtime);
        if (strftime(ftime, sizeof ftime, "%H:%M", ti) < (sizeof(ftime)-1)) {
                self->clock_id = 0;
                return FALSE;
        }
        gtk_label_set_label(GTK_LABEL(self->clock), ftime);

        return TRUE;
}

gboolean start_clock(GtkWidget *widget, __attribute__ ((unused)) GdkEventFocus *event)
{
        MainWindow *self = MAIN_WINDOW(widget);

        if (self->clock_id != 0) {
                goto end;
        }
        update_clock(self);
        self->clock_id = g_timeout_add_seconds_full(G_PRIORITY_LOW, 1, (GSourceFunc)update_clock, self, NULL);
end:
        return GDK_EVENT_PROPAGATE;
}

gboolean stop_clock(GtkWidget *widget, __attribute__ ((unused)) GdkEventFocus *event)
{
        MainWindow *self = MAIN_WINDOW(widget);

        if (self->clock_id == 0) {
                goto end;
        }

        g_source_remove(self->clock_id);
        self->clock_id = 0;
end:
        return GDK_EVENT_PROPAGATE;
}

/* Initialisation */
static void main_window_class_init(MainWindowClass *klass)
{
        GObjectClass *g_object_class;
        GtkWidgetClass *wid_class;

        g_object_class = G_OBJECT_CLASS(klass);
        wid_class = GTK_WIDGET_CLASS(klass);
        g_object_class->dispose = &main_window_dispose;

        wid_class->focus_in_event = start_clock;
        wid_class->focus_out_event = stop_clock;
}

/**
 * Get the ball rolling
 */
static void main_window_init(MainWindow *self)
{
        GtkWidget *box = NULL;
        GtkWidget *wid = NULL;
        GtkWidget *img = NULL;

        init_styles();
        gtk_window_set_title(GTK_WINDOW(self), "Columbiad - Fullscreen in Future!");
        gtk_window_set_position(GTK_WINDOW(self), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(self), 800, 600);

        /* Top box setup */
        box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
        gtk_container_add(GTK_CONTAINER(self), box);
        g_object_set(box, "halign", GTK_ALIGN_END, "valign", GTK_ALIGN_START, "margin-right", 20, "margin-top", 40, NULL);

        wid = gtk_label_new("");
        WCLASS(wid, "clock");
        gtk_box_pack_start(GTK_BOX(box), wid, FALSE, FALSE, 0);
        self->clock = wid;

        self->clock_id = g_timeout_add_seconds_full(G_PRIORITY_LOW, 1, (GSourceFunc)update_clock, self, NULL);
        update_clock(self);

        wid = gtk_menu_button_new();
        img = gtk_image_new_from_icon_name("system-shutdown-symbolic", GTK_ICON_SIZE_LARGE_TOOLBAR);
        gtk_container_add(GTK_CONTAINER(wid), img);

        gtk_widget_set_can_focus(wid, FALSE);
        gtk_button_set_relief(GTK_BUTTON(wid), GTK_RELIEF_NONE);
        gtk_box_pack_start(GTK_BOX(box), wid, FALSE, FALSE, 0);

        /* Construct top menu.. */
        self->menu = g_menu_new();
        g_menu_append(self->menu, "Quit", "app.quit");

        gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(wid), G_MENU_MODEL(self->menu));

        gtk_widget_show_all(GTK_WIDGET(self));
}

static void app_quit(GApplication *app, __attribute__ ((unused)) gpointer udata)
{
        g_application_quit(app);
}

/*
 * We need to do this post construction because the application property isn't
 * available to us.. Not at all inconvenient.
 */
void main_window_init_ui(MainWindow *self)
{
        GSimpleAction *action = NULL;
        GtkApplication *app = NULL;

        app = gtk_window_get_application(GTK_WINDOW(self));
        action = g_simple_action_new("quit", NULL);
        g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(action));
        g_object_unref(action);

        g_signal_connect_swapped(action, "activate", G_CALLBACK(app_quit), app);
}

static void main_window_dispose(GObject *object)
{
        MainWindow *self = MAIN_WINDOW(object);

        if (self->menu) {
                g_object_unref(self->menu);
                self->menu = NULL;
        }
        stop_clock(GTK_WIDGET(self), NULL);

        G_OBJECT_CLASS (main_window_parent_class)->dispose (object);
}

/* Utility; return a new MainWindow */
MainWindow *main_window_new(GtkApplication *application)
{
        MainWindow *self;

        self = g_object_new(MAIN_WINDOW_TYPE, "application", application, NULL);
        return self;
}
