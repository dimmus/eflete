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

#ifndef UI_PROPERTY_1_H
#define UI_PROPERTY_1_H

/**
 * @defgroup Property Property
 * @ingroup Block
 *
 * This widget designed for show group and part details.
 */

#include "eflete.h"

Evas_Object *
ui_property_group_add(Evas_Object *parent);

void
ui_property_group_set(Evas_Object *parent, Group *group);

void
ui_property_group_unset(Evas_Object *parent);

void
ui_property_part_set(Evas_Object *parent, Part *part);

void
ui_property_part_state_set(Evas_Object *parent, Part *part);

Evas_Object *
ui_property_image_add(Evas_Object *parent);

/* TODO: put this function into a certain place */
void
_on_frame_click(void *data,
                Evas_Object *obj,
                void *event_info);

Evas_Object *
ui_property_sound_add(Evas_Object *parent);

Evas_Object *
ui_property_style_add(Evas_Object *parent);

Evas_Object *
ui_property_color_add(Evas_Object *parent);

Evas_Object *
ui_property_demo_text_add(Evas_Object *parent);

void
ui_property_demo_text_part_set(Evas_Object *property, Part *part);

Evas_Object *
ui_property_demo_swallow_add(Evas_Object *parent);

void
ui_property_demo_swallow_part_set(Evas_Object *property, Part *part);

#endif /* UI_PROPERTY_1_H */
