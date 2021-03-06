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

typedef struct _AppSettings AppSettings;
typedef struct _AppSettingsClass AppSettingsClass;

#define APP_SETTINGS_TYPE (app_settings_get_type())
#define APP_SETTINGS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), APP_SETTINGS_TYPE, AppSettings))
#define IS_APP_SETTINGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), APP_SETTINGS_TYPE))
#define APP_SETTINGS_CLASS(klass)                                                                  \
        (G_TYPE_CHECK_CLASS_CAST((klass), APP_SETTINGS_TYPE, AppSettingsClass))
#define IS_APP_SETTINGS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), APP_SETTINGS_TYPE))
#define APP_SETTINGS_GET_CLASS(obj)                                                                \
        (G_TYPE_INSTANCE_GET_CLASS((obj), APP_SETTINGS_TYPE, AppSettingsClass))

/* AppSettings object */
struct _AppSettings {
        GObject parent;
        gint icon_size;
        gint icon_size_small;
};

/* AppSettings class definition */
struct _AppSettingsClass {
        GObjectClass parent_class;
};

GType app_settings_get_type(void);

AppSettings *app_settings_get_default(void);

gint app_settings_get_icon_size(void);
gint app_settings_get_icon_size_large(void);

#define SETTINGS_PATH "columbiad.ini"

static inline gchar *get_settings_path(void)
{
        return g_build_filename(G_DIR_SEPARATOR_S, g_get_user_config_dir(), SETTINGS_PATH, NULL);
}
