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

#include <stdbool.h>

#include "carousel.h"
#include "image.h"
#include "common.h"


G_DEFINE_TYPE(AppCarousel, app_carousel, GTK_TYPE_EVENT_BOX)

/* Boilerplate GObject code */
static void app_carousel_class_init(AppCarouselClass *klass);
static void app_carousel_init(AppCarousel *self);
static void app_carousel_dispose(GObject *object);
static gboolean key_release_event(GtkWidget *widget, GdkEventKey *key);

/* Initialisation */
static void app_carousel_class_init(AppCarouselClass *klass)
{
        GObjectClass *g_object_class;
        GtkWidgetClass *widg_class;

        g_object_class = G_OBJECT_CLASS(klass);
        widg_class = GTK_WIDGET_CLASS(klass);

        g_object_class->dispose = &app_carousel_dispose;
        widg_class->key_press_event = key_release_event;
}

static void select_item(AppCarousel *self, GList *node, bool next)
{
        LauncherImage *image = NULL;
        GtkAllocation alloc;
        GtkAdjustment *adj = NULL;
        gdouble val;
        gdouble max;
        gdouble nval = 0;

        if (self->node) {
                image = self->node->data;
                g_object_set(image, "active", FALSE, NULL);
        }
        if (!node) {
                return;
        }
        self->node = node;
        image = self->node->data;
        g_object_set(image, "active", TRUE, NULL);

        adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(self->scroll));
        val = gtk_adjustment_get_value(adj);
        max = gtk_adjustment_get_upper(adj);

        gtk_widget_get_allocation(GTK_WIDGET(image), &alloc);

        if (alloc.x > 0) {
                if (alloc.x > max) {
                        nval = val;
                } else {
                        if (next) {
                                nval = alloc.x- (alloc.width*3.5);
                        } else {
                                nval = alloc.x- (alloc.width*2.65);
                        }
                }
        }
        gtk_adjustment_set_value(adj, nval);
}

/**
 * Launch the currently selected launcher
 *
 * @param self Current instance
 */
static void activate_item(AppCarousel *self)
{
        GError *error = NULL;
        GAppInfo *info = NULL;
        LauncherImage *image = NULL;

        if (!self->node) {
                return;
        }

        image = self->node->data;
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
}

static gboolean key_release_event(GtkWidget *widget, GdkEventKey *key)
{
        AppCarousel *self = APP_CAROUSEL(widget);

        gboolean ret = GDK_EVENT_STOP;
        GList *next = NULL;
        bool next_switch = true;

        if (!self->node) {
                return GDK_EVENT_PROPAGATE;
        }

        switch (key->keyval) {
                case GDK_KEY_Right:
                        next = self->node->next;
                        break;
                case GDK_KEY_Left:
                        next = self->node->prev;
                        next_switch = false;
                        break;
                case GDK_KEY_Return:
                case GDK_KEY_KP_Enter:
                        activate_item(self);
                        break;
                default:
                        ret = GDK_EVENT_PROPAGATE;
                        break;
        }

        if (!next) {
                return ret;
        }

        select_item(self, next, next_switch);
        return ret;
}

static void build_apps(AppCarousel *self)
{
        GList *apps = NULL;
        GList *elem = NULL;
        GAppInfo *info = NULL;
        GList *children = NULL;

        if (self->children) {
                g_list_free(self->children);
                self->children = NULL;
        }

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
                image = launcher_image_new(info);
                gtk_box_pack_start(GTK_BOX(self->box), image, FALSE, FALSE, 0);
                children = g_list_append(children, image);
        }
        self->children = children;

        select_item(self, self->children, true);

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

        gtk_widget_set_size_request(GTK_WIDGET(self), -1, LARGE_PIXEL_SIZE+(DEFAULT_PIXEL_SIZE*0.5));

        gtk_widget_add_events(GTK_WIDGET(self), GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

        g_object_set(self, MARGIN_START, 10, MARGIN_END, 10, NULL);
}


static void app_carousel_dispose(GObject *object)
{
        AppCarousel *self = APP_CAROUSEL(object);

        if (self->children) {
                g_list_free(self->children);
                self->children = NULL;
        }

        G_OBJECT_CLASS (app_carousel_parent_class)->dispose (object);
}

/* Utility; return a new AppCarousel */
GtkWidget *app_carousel_new()
{
        GtkWidget *self;

        self = g_object_new(APP_CAROUSEL_TYPE, NULL);
        return self;
}
