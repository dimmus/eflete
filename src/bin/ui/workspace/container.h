/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#ifndef CONTAINER_H
#define CONTAINER_H

/**
 * @defgroup Container Container
 * @ingroup EFLETE
 *
 * The Container it's a graphical container for some object. Container object
 * has a container and two handlers. Handlers match of rel1 and rel2 point of
 * Edje object, it means top-left corner (rel1) and bottom-right corner (rel2).
 * Handler's moving will cause container size change'. Moving the handler
 * container size will be changed.
 *
 * The Container has its style. The style comprises of three edje group:
 * @li "eflete/container/base/default";
 * @li "eflete/container/handler_TL/default";
 * @li "eflete/container/handler_BR/default".
 */

#include "eflete.h"
#include "groupview.h"

struct _Container_Geom {
   int x, y, w, h, dx, dy;
};

typedef struct _Container_Geom Container_Geom;

typedef Groupview_Geom * (*Object_Protrusion_Get)(Evas_Object *obj);

/**
 * Add new Container object to parent object.
 * To delete Container object use evas_object_del.
 *
 * @param parent The parent object.
 *
 * @return The Container object.
 *
 * @ingroup Container
 */
Evas_Object *
container_add(Evas_Object *parent);

/**
 * Set the size of Container handlers.
 *
 * @param obj The Container object,
 * @param hbr_w wigth of bottom-right handler,
 * @param hbr_h height of bottom-tight handler.
 *
 * @note if trying to set the hbr_w, hbr_h < 0, will be set 5.
 *       The default handlers size is 5 (wight/hight)
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Container
 */
Eina_Bool
container_handler_size_set(Evas_Object *obj, int hrb_w, int hrb_h);

/**
 * Get the size of the Container handlers.
 *
 * @param obj The Container object,
 * @param hbr_w pointer of int width of bottom-right handler,
 * @param hbr_h pointer of int height of bottom-tight handler.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Container
 */
Eina_Bool
container_handler_size_get(Evas_Object *obj, int *hbr_w, int *hbr_h);

/**
 * Set the minimal size of container.
 *
 * @param obj The Container object,
 * @param w minimal width of container,
 * @param h minimal height of container.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if tring to set the w or h < 0, will be set 0
 *
 * @ingroup Container
 */
Eina_Bool
container_min_size_set(Evas_Object *obj, int w, int h);

/**
 * Set the maximum size of container.
 *
 * @param obj The Container object,
 * @param w maximum width of container,
 * @param h maximum height of container.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if tring to set the w or h < 0, will be set -1
 *
 * @ingroup Container
 */
Eina_Bool
container_max_size_set(Evas_Object *obj, int w, int h);

/**
 * Set the size of container.
 *
 * @param obj The Container object,
 * @param w The new width of container,
 * @param h The new height of container.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if w over the maximum weight will be set maximum width of container;
 *       if h over the maximum height will be set maximum height of container;
 *       if w less the minimum weight will be set minimum width of container;
 *       if h less the minimum height will be set minimum height of container.
 *
 * @ingroup Container
 */
Eina_Bool
container_container_size_set(Evas_Object *obj, int w, int h);

/**
 * Get the size of container.
 *
 * @param obj The Container object,
 * @param w The int pointer where width will be set,
 * @param h The int pointer where height will be set.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Container
 */
Eina_Bool
container_container_size_get(Evas_Object *obj, int *w, int *h);

/**
 * Set the new style to Container object.
 *
 * @param obj The Container object,
 * @param style The name of style that will be set.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @note if the style description missing for some Container parts (container,
 *       handlers), will be used default style.
 *
 * @ingroup Container
 */
Eina_Bool
container_style_set(Evas_Object *obj, const char *style);

/**
 * Get the current style name of Container.
 *
 * @param obj The Container object.
 *
 * @return the style name.
 *
 * @ingroup Container
 */
const char *
container_style_get(Evas_Object *obj);

/**
 * Set the content to the Container object.
 *
 * @param obj The Container object,
 * @param content The content object.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Container
 */
Eina_Bool
container_content_set(Evas_Object *obj, Evas_Object *content);

/**
 * Unset the content to the Container object.
 *
 * @param obj The Container object,
 * @param content The content object.
 *
 * @return The content object will be returned, or NULL if object not set setted.
 *
 * @ingroup Container
 */
Evas_Object *
container_content_unset(Evas_Object *obj);

/**
 * Lock the given container, hide the handler
 *
 * @param obj The Container object.
 * @param lock The lock flag, EINA_TRUE - lock, EINA_FALSE - unlock.
 *
 * @ingroup Container
 */
void
container_lock_set(Evas_Object *obj, Eina_Bool lock);

/**
 * Get the lock state
 *
 * @param obj The Container object.
 *
 * @ingroup Container
 */
Eina_Bool
container_lock_get(Evas_Object *obj);

/**
 * Get the size of paddings before top left handler and after bottom right
 * handler.
 *
 * @param obj The Container object,
 * @param htl_w pointer of int width of top-left padding,
 * @param htl_h pointer of int height of top-left padding,
 * @param hbr_w pointer of int width of bottom-right padding,
 * @param hbr_h pointer of int height of bottom-tight padding.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Container
 */
Eina_Bool
container_padding_size_get(Evas_Object *obj, int *tl_w, int *tl_h, int *br_w, int *br_h);

void
container_protrusion_func_set(Evas_Object *obj, Object_Protrusion_Get func);

/**
 * Set the size aspect.
 *
 * @param obj The Container obj,
 * @param aspect The aspect value,
 *
 * @ingroup Container
 */
void
container_aspect_set(Evas_Object *obj, double aspect);

/**
 * Get the container size aspect.
 *
 * @param obj The Container obj.
 *
 * @ingroup Container
 */
double
container_aspect_get(Evas_Object *obj);

/**
 * Set the zoom factor, this attribute have effect to resize step.
 *
 * @param obj The Container obj,
 * @param zoom The zoom factor.
 *
 * @ingroup Container
 */
void
container_zoom_factor_set(Evas_Object *obj, double zoom);

/**
 * Get the zoom factor
 *
 * @param obj The Container obj.
 *
 * @ingroup Container
 */
double
container_zoom_factor_get(Evas_Object *obj);

#endif /* CONTAINER_H */
