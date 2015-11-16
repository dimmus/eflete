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

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "string_common.h"
#include "new_history.h"
#include "editor.h"
#include "validator.h"

#include "signals.h"

#define SOUND_PROP_DATA "sound_prop_data"

#define SOUND_PROP_DATA_GET() \
   assert(property != NULL); \
   Sound_Prop_Data *pd = evas_object_data_get(property, SOUND_PROP_DATA); \
   assert(pd != NULL);

/*
 * Callback is added for frames at property box to correct scroller
 * work while each frame would be expanded/collapsed
 */
#define FRAME_PROPERTY_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

struct _Sound_Prop_Data
{
   Evas_Object *box;
   Evas_Object *sound_player;

   Evas_Object *name;
   Evas_Object *location;
   Evas_Object *type;
   Evas_Object *size;
   Evas_Object *compression;
   Evas_Object *usage_list;
};
typedef struct _Sound_Prop_Data Sound_Prop_Data;

static void
_on_sound_selected(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
}

static void
_on_tone_selected(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
}

Evas_Object *
ui_property_sound_add(Evas_Object *parent)
{
   Sound_Prop_Data *pd;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Sound_Prop_Data));

   /* editors */
   BOX_ADD(parent, pd->box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);

   pd->sound_player = elm_layout_add(parent);
   elm_layout_theme_set(pd->sound_player, "layout", "sound_editor", "player");
   elm_box_pack_end(pd->box, pd->sound_player);
   evas_object_size_hint_weight_set(pd->sound_player, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->sound_player, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(pd->sound_player);

   evas_object_data_set(pd->box, SOUND_PROP_DATA, pd);

   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_SELECTED, _on_sound_selected, pd->box);
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_SELECTED, _on_tone_selected, pd->box);

   return pd->box;
}