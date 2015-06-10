/*
 * carousel.c
 * 
 * Copyright 2015 Ikey Doherty <ikey.doherty@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "carousel.h"

/* In future do the whole maths thing to figure out a reasonable size */
#define DEFAULT_PIXEL_SIZE 128

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

static void build_apps(__attribute__ ((unused)) AppCarousel *self)
{
        GList *apps = NULL;
        GList *elem = NULL;
        GAppInfo *info = NULL;

        apps = g_app_info_get_all();
        if (!apps) {
                /* TODO: Scream at the user */
                g_warning("No apps!");
                return;
        }

        for (elem = apps; elem; elem = elem->next) {
                GtkWidget *image = NULL;
                info = elem->data;
                if (!g_app_info_should_show(info)) {
                        continue;
                }
                /* Soo.. this is very temporary.. */
                image = gtk_image_new_from_gicon(g_app_info_get_icon(info), GTK_ICON_SIZE_INVALID);
                gtk_image_set_pixel_size(GTK_IMAGE(image), DEFAULT_PIXEL_SIZE);
                g_object_set(image, "margin", 10, NULL);
                gtk_box_pack_start(GTK_BOX(self->box), image, FALSE, FALSE, 0);
        }

        g_list_free(apps);
}

static void app_carousel_init(AppCarousel *self)
{
        GtkWidget *wid = NULL;

        wid = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(self), wid);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(wid), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        self->scroll = wid;

        wid = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_container_add(GTK_CONTAINER(self->scroll), wid);
        self->box = wid;
        build_apps(self);

        gtk_widget_set_size_request(GTK_WIDGET(self), -1, DEFAULT_PIXEL_SIZE+(DEFAULT_PIXEL_SIZE*0.5));
}


static void app_carousel_dispose(GObject *object)
{
        G_OBJECT_CLASS (app_carousel_parent_class)->dispose (object);
}

/* Utility; return a new AppCarousel */
GtkWidget *app_carousel_new()
{
        GtkWidget *self;

        self = g_object_new(APP_CAROUSEL_TYPE, NULL);
        return self;
}
