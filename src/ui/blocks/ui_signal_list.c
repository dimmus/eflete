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

#include "ui_signal_list.h"

#define STYLE "style"
#define SIGNALS_LIST "sl_signal_list"

static Elm_Genlist_Item_Class *_itc_signal = NULL;

static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   Signal *sig = (Signal *)data;
   if (!strcmp(part, "elm.text"))
      return strdup(sig->name);
   if ((!strcmp(part, "elm.text.source")) && (sig->source))
      return strdup(sig->source);
   if ((!strcmp(part, "elm.text.program")) && (sig->program))
      return strdup(sig->program);

   return "";
}

static void
_on_signal_selected(void *data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Signal *sig = (Signal *)elm_object_item_data_get(glit);
   evas_object_smart_callback_call(obj, "sl,signal,select", sig);
}

Evas_Object *
ui_signal_list_add(Evas_Object *parent)
{
   Evas_Object *gl_signals;

   if (!parent) return NULL;

   if (!_itc_signal)
     {
        _itc_signal = elm_genlist_item_class_new();
        _itc_signal->item_style = "eflete/signals";
        _itc_signal->func.text_get = _item_label_get;
        _itc_signal->func.content_get = NULL;
        _itc_signal->func.state_get = NULL;
        _itc_signal->func.del = NULL;
     }

   gl_signals = elm_genlist_add(parent);
   elm_object_style_set(gl_signals, "eflete/default");
   evas_object_size_hint_align_set(gl_signals, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_signals, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   return gl_signals;
}

Eina_Bool
ui_signal_list_data_set(Evas_Object *object, Style *style)
{
   Eina_List *signals, *l;
   Signal *sig = NULL;
   Style *tmp;
   Elm_Object_Item *it = NULL;

   if ((!object) || (!style) || (!style->obj)) return false;

   tmp = evas_object_data_get(object, STYLE);
   if (tmp) evas_object_data_del(object, STYLE);

   signals = wm_program_signals_list_get(style);
   EINA_LIST_FOREACH(signals, l, sig)
     {
        it = elm_genlist_item_append(object, _itc_signal,
                                sig, NULL, ELM_GENLIST_ITEM_NONE,
                                _on_signal_selected, NULL);
        elm_genlist_item_select_mode_set(it, ELM_OBJECT_SELECT_MODE_ALWAYS);
        elm_object_item_data_set(it, sig);
     }
   evas_object_data_set(object, STYLE, style);
   evas_object_data_set(object, SIGNALS_LIST, signals);

   return true;
}

Style *
ui_signal_list_data_unset(Evas_Object *object)
{
   Style *style;
   if (!object) return NULL;

   Eina_List *signals = evas_object_data_del(object, SIGNALS_LIST);
   wm_program_signals_list_free(signals);

   style = evas_object_data_get(object, STYLE);
   if (style) evas_object_data_del(object, STYLE);

   elm_genlist_clear(object);

   return style;
}
