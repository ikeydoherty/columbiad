/*
 * carousel.h
 * 
 * Copyright 2015 Ikey Doherty <ikey.doherty@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 */

#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

typedef struct _AppCarousel AppCarousel;
typedef struct _AppCarouselClass   AppCarouselClass;

#define APP_CAROUSEL_TYPE (app_carousel_get_type())
#define APP_CAROUSEL(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), APP_CAROUSEL_TYPE, AppCarousel))
#define IS_APP_CAROUSEL(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), APP_CAROUSEL_TYPE))
#define APP_CAROUSEL_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), APP_CAROUSEL_TYPE, AppCarouselClass))
#define IS_APP_CAROUSEL_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), APP_CAROUSEL_TYPE))
#define APP_CAROUSEL_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), APP_CAROUSEL_TYPE, AppCarouselClass))

/* AppCarousel object */
struct _AppCarousel {
        GtkEventBox parent;
        GtkWidget *scroll;
        GtkWidget *box;
};

/* AppCarousel class definition */
struct _AppCarouselClass {
        GtkEventBoxClass parent_class;
};

GType app_carousel_get_type(void);

/**
 * Construct a new AppCarousel
 *
 * @return A new AppCarousel
 */
GtkWidget *app_carousel_new(void);
