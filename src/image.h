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

typedef struct _LauncherImage LauncherImage;
typedef struct _LauncherImageClass LauncherImageClass;

#define LAUNCHER_IMAGE_TYPE (launcher_image_get_type())
#define LAUNCHER_IMAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAUNCHER_IMAGE_TYPE, LauncherImage))
#define IS_LAUNCHER_IMAGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAUNCHER_IMAGE_TYPE))
#define LAUNCHER_IMAGE_CLASS(klass)                                                                \
        (G_TYPE_CHECK_CLASS_CAST((klass), LAUNCHER_IMAGE_TYPE, LauncherImageClass))
#define IS_LAUNCHER_IMAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAUNCHER_IMAGE_TYPE))
#define LAUNCHER_IMAGE_GET_CLASS(obj)                                                              \
        (G_TYPE_INSTANCE_GET_CLASS((obj), LAUNCHER_IMAGE_TYPE, LauncherImageClass))

/* LauncherImage object */
struct _LauncherImage {
        GtkBox parent;
        GtkWidget *label;
        GtkWidget *image;
        GAppInfo *info;
        gboolean active;

        gint count;
};

/* LauncherImage class definition */
struct _LauncherImageClass {
        GtkBoxClass parent_class;
};

GType launcher_image_get_type(void);

/**
 * Construct a new LauncherImage
 *
 * @param info A GAppInfo instance to bind to
 * @return A new LauncherImage
 */
GtkWidget *launcher_image_new(GAppInfo *info);
