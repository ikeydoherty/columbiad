/*
 * image.c
 * 
 * Copyright 2015 Ikey Doherty <ikey.doherty@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "image.h"

/* In future do the whole maths thing to figure out a reasonable size */
#define DEFAULT_PIXEL_SIZE 128

G_DEFINE_TYPE(LauncherImage, launcher_image, GTK_TYPE_EVENT_BOX)

/* Boilerplate GObject code */
static void launcher_image_class_init(LauncherImageClass *klass);
static void launcher_image_init(LauncherImage *self);
static void launcher_image_dispose(GObject *object);
static void update_app_info(LauncherImage *self);

enum {
        PROP_MIN, PROP_APPINFO, N_PROPERTIES
};

static GParamSpec *obj_props[N_PROPERTIES] = { NULL, };

static void launcher_image_set_property(GObject *object,
                                           guint prop_id,
                                           const GValue *value,
                                           GParamSpec *pspec)
{
        LauncherImage *self;

        self = LAUNCHER_IMAGE(object);
        switch (prop_id) {
                case PROP_APPINFO:
                        self->info = g_value_get_pointer((GValue*)value);
                        if (!self->info) {
                                return;
                        }
                        update_app_info(self);
                        break;
                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                prop_id, pspec);
                        break;
        }
}

static void launcher_image_get_property(GObject *object,
                                           guint prop_id,
                                           GValue *value,
                                           GParamSpec *pspec)
{
        LauncherImage *self;

        self = LAUNCHER_IMAGE(object);
        switch (prop_id) {
                case PROP_APPINFO:
                        g_value_set_pointer((GValue *)value, self->info);
                        break;
                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                prop_id, pspec);
                        break;
        }
}


/* Initialisation */
static void launcher_image_class_init(LauncherImageClass *klass)
{
        GObjectClass *g_object_class;
        g_object_class = G_OBJECT_CLASS(klass);

        g_object_class->dispose = &launcher_image_dispose;

        obj_props[PROP_APPINFO] =
                g_param_spec_pointer("appinfo", "AppInfo", "AppInfo",
                        G_PARAM_CONSTRUCT | G_PARAM_WRITABLE);

        g_object_class->set_property = launcher_image_set_property;
        g_object_class->get_property = launcher_image_get_property;

        g_object_class_install_properties(g_object_class, N_PROPERTIES, obj_props);
}

static void launcher_image_init(LauncherImage *self)
{
        GtkWidget *image = NULL;

        image = gtk_image_new();
        gtk_container_add(GTK_CONTAINER(self), image);
        g_object_set(image, "margin", 10, NULL);
        self->image = image;
}

static void update_app_info(LauncherImage *self)
{
        if (!self->info) {
                return;
        }
        gtk_image_set_from_gicon(GTK_IMAGE(self->image), g_app_info_get_icon(self->info), GTK_ICON_SIZE_INVALID);
        gtk_image_set_pixel_size(GTK_IMAGE(self->image), DEFAULT_PIXEL_SIZE);
}

static void launcher_image_dispose(GObject *object)
{
        G_OBJECT_CLASS (launcher_image_parent_class)->dispose (object);
}

/* Utility; return a new LauncherImage */
GtkWidget *launcher_image_new(GAppInfo *info)
{
        GtkWidget *self;

        self = g_object_new(LAUNCHER_IMAGE_TYPE, "appinfo", info, NULL);
        return self;
}
