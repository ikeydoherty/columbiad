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
#include "common.h"
#include "settings.h"

G_DEFINE_TYPE(LauncherImage, launcher_image, GTK_TYPE_BOX)

/* Boilerplate GObject code */
static void launcher_image_class_init(LauncherImageClass *klass);
static void launcher_image_init(LauncherImage *self);
static void launcher_image_dispose(GObject *object);
static void update_app_info(LauncherImage *self);
static void update_appearance(LauncherImage *self);

enum {
        PROP_MIN, PROP_APPINFO, PROP_ACTIVE, N_PROPERTIES
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
                case PROP_ACTIVE:
                        self->active = g_value_get_boolean((GValue*)value);
                        update_appearance(self);
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
                        g_value_set_pointer((GValue*)value, self->info);
                        break;
                case PROP_ACTIVE:
                        g_value_set_boolean((GValue*)value, self->active);
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
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
        obj_props[PROP_ACTIVE] =
                g_param_spec_boolean("active", "Active", "Active",  FALSE,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

        g_object_class->set_property = launcher_image_set_property;
        g_object_class->get_property = launcher_image_get_property;

        g_object_class_install_properties(g_object_class, N_PROPERTIES, obj_props);
}

static void launcher_image_init(LauncherImage *self)
{
        GtkWidget *image = NULL, *label = NULL;

        image = gtk_image_new();
        gtk_box_pack_start(GTK_BOX(self), image, FALSE, FALSE, 0);
        g_object_set(image, "margin", 10, NULL);
        self->image = image;

        gtk_widget_set_halign(image, GTK_ALIGN_START);

        label = gtk_label_new("");
        WCLASS(label, "launcher");
        gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
        gtk_box_pack_start(GTK_BOX(self), label, FALSE, FALSE, 1);
        self->label = label;
}

static void update_app_info(LauncherImage *self)
{
        if (!self->info) {
                return;
        }
        gtk_image_set_from_gicon(GTK_IMAGE(self->image), g_app_info_get_icon(self->info), GTK_ICON_SIZE_DIALOG);
        gtk_image_set_pixel_size(GTK_IMAGE(self->image), app_settings_get_icon_size());

        gtk_label_set_text(GTK_LABEL(self->label), g_app_info_get_display_name(self->info));
}

/**
 * Update current appearance/size:
 *
 * @TODO: Tween this visual change
 */
static void update_appearance(LauncherImage *self)
{
        if (!self->info) {
                return;
        }
        if (self->active) {
                gtk_image_set_pixel_size(GTK_IMAGE(self->image), app_settings_get_icon_size_large());
                WCLASS(self->label, "active-text");
        } else {
                gtk_image_set_pixel_size(GTK_IMAGE(self->image), app_settings_get_icon_size());
                WRMCLASS(self->label, "active-text");
        }
}

static void launcher_image_dispose(GObject *object)
{
        G_OBJECT_CLASS (launcher_image_parent_class)->dispose (object);
}

/* Utility; return a new LauncherImage */
GtkWidget *launcher_image_new(GAppInfo *info)
{
        GtkWidget *self;

        self = g_object_new(LAUNCHER_IMAGE_TYPE, "orientation", GTK_ORIENTATION_VERTICAL, "appinfo", info, NULL);
        return self;
}
