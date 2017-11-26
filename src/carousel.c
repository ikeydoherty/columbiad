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

#include <stdbool.h>

#include "carousel.h"
#include "common.h"
#include "image.h"
#include "settings.h"

G_DEFINE_TYPE(AppCarousel, app_carousel, GTK_TYPE_EVENT_BOX)

/* Boilerplate GObject code */
static void app_carousel_class_init(AppCarouselClass *klass);
static void app_carousel_init(AppCarousel *self);
static void app_carousel_dispose(GObject *object);

/* Initialisation */
static void app_carousel_class_init(AppCarouselClass *klass)
{
        GObjectClass *g_object_class;

        g_object_class = G_OBJECT_CLASS(klass);

        g_object_class->dispose = &app_carousel_dispose;
}

static void selection_changed(AppCarousel *self, GtkFlowBox *box)
{
        GList *children = NULL;
        GtkWidget *image = NULL;

        if (self->selected) {
                g_object_set(self->selected, "active", FALSE, NULL);
        }

        children = gtk_flow_box_get_selected_children(box);
        if (!children) {
                return;
        }

        image = gtk_bin_get_child(GTK_BIN(children->data));
        g_object_set(image, "active", TRUE, NULL);
        self->selected = image;

        g_list_free(children);
}

static gint sort_items(GtkFlowBoxChild *a, GtkFlowBoxChild *b, __attribute__((unused)) gpointer v)
{
        LauncherImage *l = NULL, *r = NULL;

        l = LAUNCHER_IMAGE(gtk_bin_get_child(GTK_BIN(a)));
        r = LAUNCHER_IMAGE(gtk_bin_get_child(GTK_BIN(b)));

        return (r->count > l->count) - (r->count < l->count);
}

static void activate_child(AppCarousel *self, GtkFlowBoxChild *child, GtkFlowBox *box)
{
        GError *error = NULL;
        GAppInfo *info = NULL;
        LauncherImage *image = NULL;

        image = LAUNCHER_IMAGE(gtk_bin_get_child(GTK_BIN(child)));
        g_object_get(image, "appinfo", &info, NULL);
        if (!info) {
                fprintf(stderr, "Launcher is missing appinfo. Kinda fatal.\n");
                return;
        }

        if (!g_app_info_launch(info, NULL, NULL, &error)) {
                fprintf(stderr, "Error launching app: %s\n", error->message);
        }
        if (error) {
                g_error_free(error);
        }
        image->count++;

        gtk_flow_box_invalidate_sort(box);
        gtk_flow_box_select_child(box, child);

        /* Perhaps optimise?? */
        app_carousel_write_config(self);
}

static void build_apps(AppCarousel *self)
{
        GList *apps = NULL;
        GList *elem = NULL;
        GAppInfo *info = NULL;
        GError *error = NULL;

        apps = g_app_info_get_all();
        if (!apps) {
                /* TODO: Scream at the user */
                g_warning("No apps!");
                return;
        }

        for (elem = apps; elem; elem = elem->next) {
                GtkWidget *image = NULL;
                const gchar *id = NULL;
                info = elem->data;
                if (!g_app_info_should_show(info)) {
                        continue;
                }
                image = launcher_image_new(info);
                id = g_app_info_get_id(info);
                gtk_container_add(GTK_CONTAINER(self->box), image);

                /* Set the count if we have it.. */
                if (self->config && g_key_file_has_key(self->config, "Counts", id, NULL)) {
                        gint count = g_key_file_get_integer(self->config, "Counts", id, &error);
                        if (error) {
                                g_printerr("Error loading int: [Counts] %s\n", error->message);
                                g_error_free(error);
                                continue;
                        }
                        LAUNCHER_IMAGE(image)->count = count;
                }
        }

        g_list_free(apps);
}

void app_carousel_write_config(AppCarousel *self)
{
        g_autoptr(GError) error = NULL;
        g_autofree gchar *path = NULL;
        GKeyFile *config = NULL;
        GList *children = NULL, *em = NULL;
        LauncherImage *image = NULL;
        const gchar *id = NULL;

        path = get_settings_path();
        config = g_key_file_new();

        children = gtk_container_get_children(GTK_CONTAINER(self->box));

        for (em = children; em; em = em->next) {
                image = LAUNCHER_IMAGE(gtk_bin_get_child(GTK_BIN(em->data)));
                id = g_app_info_get_id(image->info);
                if (image->count == 0) {
                        continue;
                }
                g_key_file_set_integer(config, "Counts", id, image->count);
        }

        if (!g_key_file_save_to_file(config, path, &error)) {
                g_printerr("Unable to write config: %s", error->message);
        }

        g_key_file_unref(config);
}

static void app_carousel_init(AppCarousel *self)
{
        GtkWidget *wid = NULL;
        GKeyFile *config = NULL;
        GError *error = NULL;
        g_autofree gchar *path = NULL;

        config = g_key_file_new();
        path = get_settings_path();
        self->config = config;

        if (g_file_test(path, G_FILE_TEST_EXISTS) &&
            !g_key_file_load_from_file(config, path, G_KEY_FILE_NONE, &error)) {
                g_printerr("Error loading configuration: %s\n", error->message);
                g_error_free(error);
        }

        wid = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(self), wid);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(wid),
                                       GTK_POLICY_AUTOMATIC,
                                       GTK_POLICY_AUTOMATIC);
        self->scroll = wid;

        wid = gtk_flow_box_new();
        gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(wid), 1);
        gtk_orientable_set_orientation(GTK_ORIENTABLE(wid), GTK_ORIENTATION_VERTICAL);
        gtk_container_add(GTK_CONTAINER(self->scroll), wid);
        gtk_flow_box_set_sort_func(GTK_FLOW_BOX(wid), sort_items, NULL, NULL);
        self->box = wid;
        build_apps(self);

        gtk_flow_box_invalidate_sort(GTK_FLOW_BOX(self->box));

        g_signal_connect_swapped(wid, "child-activated", G_CALLBACK(activate_child), self);
        g_signal_connect_swapped(wid,
                                 "selected-children-changed",
                                 G_CALLBACK(selection_changed),
                                 self);
        gtk_widget_set_size_request(GTK_WIDGET(self),
                                    -1,
                                    app_settings_get_icon_size_large() +
                                        (gint)(app_settings_get_icon_size() * 0.5));

        gtk_widget_add_events(GTK_WIDGET(self), GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

        g_object_set(self, MARGIN_START, 10, MARGIN_END, 10, NULL);
}

static void app_carousel_dispose(GObject *object)
{
        AppCarousel *self = APP_CAROUSEL(object);

        if (self->config) {
                g_key_file_unref(self->config);
                self->config = NULL;
        }

        G_OBJECT_CLASS(app_carousel_parent_class)->dispose(object);
}

/* Utility; return a new AppCarousel */
GtkWidget *app_carousel_new()
{
        GtkWidget *self;

        self = g_object_new(APP_CAROUSEL_TYPE, NULL);
        return self;
}
