/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "live_elementary_widgets.h"

static void
_entry_set_params(Evas_Object *obj, Eina_Stringshare *class_name)
{
   assert(obj != NULL);
   assert(class_name != NULL);

   if (strstr(class_name, "single"))
     elm_entry_single_line_set(obj, true);
   else
     elm_entry_single_line_set(obj, false);

   if (strstr(class_name, "noedit"))
     elm_entry_editable_set(obj, false);
   else
     elm_entry_editable_set(obj, true);

   if (strstr(class_name, "password"))
     elm_entry_password_set(obj, true);
   else
     elm_entry_password_set(obj, false);

   /* this checking is according to elm_entry.c:574
      function _elm_entry_theme_group_get */
   if (!strstr(class_name, "password") && !(strstr(class_name, "single")))
     {
        if (strstr(class_name, "charwrap"))
          elm_entry_line_wrap_set(obj, ELM_WRAP_CHAR);
        else if (strstr(class_name, "mixedwrap"))
          elm_entry_line_wrap_set(obj, ELM_WRAP_MIXED);
        else if (strstr(class_name, "nowrap"))
          elm_entry_line_wrap_set(obj, ELM_WRAP_NONE);
        else
          elm_entry_line_wrap_set(obj, ELM_WRAP_WORD);
     }

   if (!strcmp(class_name, "handler"))
     elm_entry_selection_handler_disabled_set(obj, false);
}

Evas_Object *
widget_entry_create(Evas_Object *parent, const Style *style)
{
   assert(parent != NULL);
   assert(style != NULL);

   Eina_Stringshare *class;
   Eina_Stringshare *style_name;
   standard_widget_name_parse(style->full_group_name, NULL, &class, &style_name);

   Evas_Object *object = elm_entry_add(parent);

   elm_object_style_set(object, style_name);
   _entry_set_params(object, class);

   elm_object_text_set(object, "This is an entry widget in this window that "
            "uses markup <b>like this</> for styling and "
            "formatting <em>like this</>, as well as "
            "<a href=X><link>links in the text</></a>, so enter text "
            "in here to edit it. By the way, links are "
            "called <a href=anc-02>Anchors</a> so you will need "
            "to refer to them this way. "
            " "

            "Also you can stick in items with (relsize + ascent): "
            "<item relsize=16x16 vsize=ascent href=emoticon/evil-laugh></item>"
            " (full) "
            "<item relsize=16x16 vsize=full href=emoticon/guilty-smile></item>"
            " (to the left) "

            "Also (size + ascent): "
            "<item size=16x16 vsize=ascent href=emoticon/haha></item>"
            " (full) "
            "<item size=16x16 vsize=full href=emoticon/happy-panting></item>"
            " (before this) "

            "And as well (absize + ascent): "
            "<item absize=64x64 vsize=ascent href=emoticon/knowing-grin></item>"
            " (full) "
            "<item absize=64x64 vsize=full href=emoticon/not-impressed></item>"
            " ... end.");

   if (strcmp(class, "emoticon") == 0)
     {
        char **parsed = NULL;
        unsigned int count_split = 0;
        Eina_Stringshare *emoticon_type = NULL;

        parsed = eina_str_split_full(style_name, "/", 2, &count_split);
        if (count_split == 2)
          {
             emoticon_type = eina_stringshare_add(parsed[0]);
             eina_stringshare_replace(&style_name, parsed[1]);
          }
        free(parsed[0]);
        free(parsed);

        char buf[4096];
        snprintf(buf, sizeof(buf), "This is an example of emoticon "
                            "<item size=16x16 vsize=ascent href=emoticon/%s></item>"
                            " some texti "
                            "<item size=16x16 vsize=full href=emoticon/%s></item>"
                            " some text "
                            "<item absize=64x64 vsize=ascent href=emoticon/%s></item>"
                            " some text "
                            "<item absize=64x64 vsize=full href=emoticon/%s></item>"
                            " ... end.", emoticon_type, emoticon_type, emoticon_type, emoticon_type);

        elm_object_text_set(object, buf);
        eina_stringshare_del(emoticon_type);
     }

   evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   eina_stringshare_del(class);
   eina_stringshare_del(style_name);
   return object;
}
