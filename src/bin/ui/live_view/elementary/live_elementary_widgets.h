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

#ifndef LIVE_ELEMENTARY_WIDGETS_H
#define LIVE_ELEMENTARY_WIDGETS_H

/**
 * @defgroup Live_Elementary_Widgets Live_Elementary_Widgets
 *
 * Header which contains all private functions required for all widgets in this folder.
 * This folder mostly contains only Elementary Widgets
 */

#include "eflete.h"
#include "project_manager.h"
#include "live_widgets.h"
#include "demo_group.h"
#include "widget_macro.h"

/**
 * Common Swallow callback that is used in most of simple widgets.
 *
 * @param data data from callback register.
 * @param obj object that generates signal that cause this callback.
 * @param event_info information about event.
 *
 * @ingroup Live_Elementary_Widgets
 */
void
on_swallow_check(void *data, Evas_Object *obj, void *event_info);

/**
 * Common Text callback that is used in most of simple widgets.
 *
 * @param data data from callback register.
 * @param obj object that generates signal that cause this callback.
 * @param event_info information about event.
 *
 * @ingroup Live_Elementary_Widgets
 */
void
on_text_check(void *data, Evas_Object *obj, void *event_info);

/**
 * Common Signal callback that is used for sending signals into widgets.
 *
 * @param data data from callback register.
 * @param obj object that generates signal that cause this callback.
 * @param event_info information about event.
 *
 * @ingroup Live_Elementary_Widgets
 */
void
send_signal(void *data, Evas_Object *obj, void *event_info);

/*****************************************************
 **************** CREATING WIDGETS *******************
 * !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  ! *
 * ! *** Those undocumented functions are same *** ! *
 * ! *** and they are private, not public API  *** ! *
 * !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  ! *
 *****************************************************/
Evas_Object *
layout_custom_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_actionslider_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_bg_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_bubble_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_button_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_calendar_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_check_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_clock_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_ctxpopup_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_datetime_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_entry_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_frame_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_gengrid_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_genlist_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_label_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_layout_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_list_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_map_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_multibuttonentry_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_notify_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_panel_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_panes_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_popup_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_radio_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_scroller_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_segment_control_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_separator_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_slider_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_spinner_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_toolbar_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_progressbar_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_naviframe_create(Evas_Object *parent, const Group *group);

Evas_Object *
widget_colorselector_create(Evas_Object *parent, const Group *group);

/*****************************************************
 *****************************************************
 *****************************************************/

#endif /* LIVE_ELEMENTARY_WIDGETS_H */
