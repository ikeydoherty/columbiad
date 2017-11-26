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

#include "settings.h"
#include "common.h"

#define DEFAULT_PIXEL_SIZE 160
#define LARGE_PIXEL_SIZE 256

G_DEFINE_TYPE(AppSettings, app_settings, G_TYPE_OBJECT)

/* Boilerplate GObject code */
static void app_settings_class_init(AppSettingsClass *klass);
static void app_settings_init(AppSettings *self);
static void app_settings_dispose(GObject *object);

enum { PROP_MIN, PROP_ICONSIZE, PROP_ICONSIZE_SMALL, N_PROPERTIES };

static GParamSpec *obj_props[N_PROPERTIES] = {
        NULL,
};

static void app_settings_set_property(GObject *object, guint prop_id, const GValue *value,
                                      GParamSpec *pspec)
{
        AppSettings *self;

        self = APP_SETTINGS(object);
        switch (prop_id) {
        case PROP_ICONSIZE:
                self->icon_size = g_value_get_int((GValue *)value);
                break;
        case PROP_ICONSIZE_SMALL:
                self->icon_size_small = g_value_get_int((GValue *)value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
                break;
        }
}

static void app_settings_get_property(GObject *object, guint prop_id, GValue *value,
                                      GParamSpec *pspec)
{
        AppSettings *self;

        self = APP_SETTINGS(object);
        switch (prop_id) {
        case PROP_ICONSIZE:
                g_value_set_int((GValue *)value, self->icon_size);
                break;
        case PROP_ICONSIZE_SMALL:
                g_value_set_int((GValue *)value, self->icon_size_small);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
                break;
        }
}

/* Initialisation */
static void app_settings_class_init(AppSettingsClass *klass)
{
        GObjectClass *g_object_class;
        g_object_class = G_OBJECT_CLASS(klass);

        g_object_class->dispose = &app_settings_dispose;

        obj_props[PROP_ICONSIZE] = g_param_spec_int("icon-size",
                                                    "IconSize",
                                                    "IconSize",
                                                    0,
                                                    1000,
                                                    LARGE_PIXEL_SIZE,
                                                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
        obj_props[PROP_ICONSIZE_SMALL] = g_param_spec_int("icon-size-small",
                                                          "IconSizeSmall",
                                                          "IconSizeSmall",
                                                          0,
                                                          1000,
                                                          DEFAULT_PIXEL_SIZE,
                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

        g_object_class->set_property = app_settings_set_property;
        g_object_class->get_property = app_settings_get_property;

        g_object_class_install_properties(g_object_class, N_PROPERTIES, obj_props);
}

static void app_settings_init(AppSettings *self)
{
        self->icon_size_small = DEFAULT_PIXEL_SIZE;
        self->icon_size = LARGE_PIXEL_SIZE;
}

static void app_settings_dispose(GObject *object)
{
        G_OBJECT_CLASS(app_settings_parent_class)->dispose(object);
}

gint app_settings_get_icon_size(void)
{
        return app_settings_get_default()->icon_size_small;
}

gint app_settings_get_icon_size_large(void)
{
        return app_settings_get_default()->icon_size;
}

/* Utility; return a new AppSettings */
AppSettings *app_settings_new(void)
{
        AppSettings *self;

        self = g_object_new(APP_SETTINGS_TYPE, NULL);
        return self;
}

static AppSettings *_instance = NULL;

AppSettings *app_settings_get_default(void)
{
        if (!_instance) {
                _instance = app_settings_new();
        }
        return _instance;
}

__attribute__((destructor)) static void app_settings_cleanup(void)
{
        g_clear_object(&_instance);
}
