/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "ui_signal_list.h"

#define STYLE "style"

static Elm_Genlist_Item_Class *_itc_signal = NULL;

static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   return strdup(data);
}

static void
_on_signal_selected(void *data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   evas_object_smart_callback_call(obj, "sl,signal,select", glit);
}

static void
_item_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   eina_stringshare_del((Eina_Stringshare *)data);
}

Evas_Object *
ui_signal_list_add(Evas_Object *parent)
{
   Evas_Object *gl_signals;

   if (!parent) return NULL;

   if (!_itc_signal)
     {
        _itc_signal = elm_genlist_item_class_new();
        _itc_signal->item_style = "eflete/default";
        _itc_signal->func.text_get = _item_label_get;
        _itc_signal->func.content_get = NULL;
        _itc_signal->func.state_get = NULL;
        _itc_signal->func.del = _item_del;
     }

   gl_signals = elm_genlist_add(parent);
   elm_object_style_set(gl_signals, "eflete/default");
   evas_object_size_hint_align_set(gl_signals, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_signals, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(gl_signals, "selected",
                                 _on_signal_selected, NULL);

   return gl_signals;
}

Eina_Bool
ui_signal_list_data_set(Evas_Object *object, Style *style)
{
   Eina_List *signals, *l;
   Style *tmp;
   Eina_Stringshare *signal;

   if ((!object) || (!style) || (!style->obj)) return false;

   tmp = evas_object_data_get(object, STYLE);
   if (tmp) evas_object_data_del(object, STYLE);

   signals = wm_program_signals_list_get(style);
   EINA_LIST_FOREACH(signals, l, signal)
     {
        elm_genlist_item_append(object, _itc_signal,
                                eina_stringshare_add(signal),
                                NULL, ELM_GENLIST_ITEM_NONE,
                                NULL, NULL);
     }
   wm_program_signals_list_free(signals);
   evas_object_data_set(object, STYLE, style);

   return true;
}

Style *
ui_signal_list_data_unset(Evas_Object *object)
{
   Style *style;
   if (!object) return NULL;

   style = evas_object_data_get(object, STYLE);
   if (style) evas_object_data_del(object, STYLE);

   elm_genlist_clear(object);

   return style;
}
