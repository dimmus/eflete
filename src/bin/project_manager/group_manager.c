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

#include "project_manager.h"
#include "widget_list.h"

Eina_List *
_strings_list_duplicates_del(Eina_List *list)
{
   Eina_List *l, *l2;
   Eina_Stringshare *str, *str2;

   EINA_LIST_FOREACH(list, l, str)
     {
        EINA_LIST_FOREACH(eina_list_next(l), l2, str2)
          {
             if ((str == str2) || !strcmp(str, str2))
               {
                  list = eina_list_remove_list(list, l2);
                  eina_stringshare_del(str2);
               }
          }
     }
   return list;
}

#define SOURCE_GET(FUNC) \
do { \
   source = FUNC(obj, part); \
   if (source) \
     { \
        groups = eina_list_sorted_insert(groups, sort_cb, eina_stringshare_add(source)); \
        eina_stringshare_del(source); \
     } \
   } \
while (0);

Eina_List *
gm_group_used_groups_edj_get(Evas_Object *obj)
{
   Eina_List *groups = NULL;
   Eina_List *parts, *l1;
   Eina_Stringshare *part, *source;
   Edje_Part_Type type;
   int items_count, i;

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        type = edje_edit_part_type_get(obj, part);
        if ((EDJE_PART_TYPE_GROUP != type) && (EDJE_PART_TYPE_TEXTBLOCK != type) && (EDJE_PART_TYPE_BOX != type) && (EDJE_PART_TYPE_TABLE != type))
          continue;
        SOURCE_GET(edje_edit_part_source_get)
        SOURCE_GET(edje_edit_part_source2_get)
        SOURCE_GET(edje_edit_part_source3_get)
        SOURCE_GET(edje_edit_part_source4_get)
        SOURCE_GET(edje_edit_part_source5_get)
        SOURCE_GET(edje_edit_part_source6_get)

        items_count = edje_edit_part_items_count_get(obj, part);
        for (i = 0; i < items_count; i++)
          {
             source = edje_edit_part_item_index_source_get(obj, part, i);
             if (source)
               {
                  groups = eina_list_sorted_insert(groups, sort_cb, eina_stringshare_add(source));
                  eina_stringshare_del(source);
               }
          }
     }
   edje_edit_string_list_free(parts);

   return _strings_list_duplicates_del(groups);
}
#undef SOURCE_GET

Eina_List *
gm_group_used_images_edj_get(Evas_Object *obj)
{
   Eina_List *images = NULL;
   Eina_List *parts, *l1, *states, *l2, *tweens;
   Eina_Stringshare *part, *state, *img, *pstate;
   double pvalue;

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        if (EDJE_PART_TYPE_IMAGE != edje_edit_part_type_get(obj, part)) continue;
        states = edje_edit_part_states_list_get(obj, part);
        EINA_LIST_FOREACH(states, l2, state)
          {
             state_name_split(state, &pstate, &pvalue);
             img = edje_edit_state_image_get(obj, part, pstate, pvalue);
             images = eina_list_sorted_insert(images, sort_cb, eina_stringshare_add(img));
             eina_stringshare_del(img);
             eina_stringshare_del(pstate);

             tweens = edje_edit_state_tweens_list_get(obj, part, pstate, pvalue);
             images = eina_list_sorted_merge(images, tweens, sort_cb);
          }
     }
   edje_edit_string_list_free(parts);

   return _strings_list_duplicates_del(images);
}

Eina_List *
gm_group_used_sample_edj_get(Evas_Object *obj)
{
   Eina_List *samples = NULL;
   Eina_List *programs, *l1;
   Eina_Stringshare *program, *sample;

   programs = edje_edit_programs_list_get(obj);
   EINA_LIST_FOREACH(programs, l1, program)
     {
        if (EDJE_ACTION_TYPE_SOUND_SAMPLE != edje_edit_program_action_get(obj, program))
          continue;

        sample = edje_edit_program_sample_name_get(obj, program);
        if (sample)
          {
             samples = eina_list_sorted_insert(samples, sort_cb, eina_stringshare_add(sample));
             eina_stringshare_del(sample);
          }
     }
   edje_edit_string_list_free(programs);

   return _strings_list_duplicates_del(samples);
}

Eina_List *
gm_group_used_color_classes_edj_get(Evas_Object *obj)
{
   Eina_List *color_classes = NULL;
   Eina_List *parts, *l1, *states, *l2;
   Eina_Stringshare *part, *state, *color_c, *pstate;
   double pvalue;

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        states = edje_edit_part_states_list_get(obj, part);
        EINA_LIST_FOREACH(states, l2, state)
          {
             state_name_split(state, &pstate, &pvalue);
             color_c = edje_edit_state_color_class_get(obj, part, pstate, pvalue);
             if (color_c)
               {
                  color_classes = eina_list_sorted_insert(color_classes, sort_cb, eina_stringshare_add(color_c));
                  eina_stringshare_del(color_c);
               }
             eina_stringshare_del(pstate);
          }
     }
   edje_edit_string_list_free(parts);

   return _strings_list_duplicates_del(color_classes);
}

Eina_List *
gm_group_used_styles_edj_get(Evas_Object *obj)
{
   Eina_List *styles = NULL;
   Eina_List *parts, *l1, *states, *l2;
   Eina_Stringshare *part, *state, *style, *pstate;
   double pvalue;

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        states = edje_edit_part_states_list_get(obj, part);
        EINA_LIST_FOREACH(states, l2, state)
          {
             if (EDJE_PART_TYPE_TEXTBLOCK != edje_edit_part_type_get(obj, part)) continue;
             state_name_split(state, &pstate, &pvalue);
             style = edje_edit_state_text_style_get(obj, part, pstate, pvalue);
             if (style)
               {
                  styles = eina_list_sorted_insert(styles, sort_cb, eina_stringshare_add(style));
                  eina_stringshare_del(style);
               }
             eina_stringshare_del(pstate);
          }
     }
   edje_edit_string_list_free(parts);

   return _strings_list_duplicates_del(styles);
}

static Eina_List *
_style_font_get(Evas_Object *obj, const char *style)
{
   Eina_List *fonts = NULL;
   Eina_List *tags, *l;
   Eina_Stringshare *font, *tag_value;
   char *pch;

   tags = edje_edit_style_tags_list_get(obj, style);
   EINA_LIST_FOREACH(tags, l, tag_value)
     {
        pch = strstr(tag_value, "font");
        if (!pch) continue;
        pch += strlen("font");
        pch = strtok(pch, " ");
        font = edje_edit_font_path_get(obj, pch);
        if (font)
          {
             fonts = eina_list_sorted_insert(fonts, sort_cb, font);
             eina_stringshare_del(font);
          }
     }

   return fonts;
}

Eina_List *
gm_group_used_fonts_edj_get(Evas_Object *obj)
{
   Eina_List *fonts = NULL;
   Eina_List *parts, *l1, *states, *l2, *style_fonts;
   Eina_Stringshare *part, *state, *font, *style, *real_font, *pstate;
   double pvalue;
   Edje_Part_Type type;

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        type = edje_edit_part_type_get(obj, part);
        states = edje_edit_part_states_list_get(obj, part);
        EINA_LIST_FOREACH(states, l2, state)
          {
             state_name_split(state, &pstate, &pvalue);
             if (EDJE_PART_TYPE_TEXT == type)
               {
                  font = edje_edit_state_font_get(obj, part, pstate, pvalue);
                  if (font)
                    {
                       real_font = edje_edit_font_path_get(obj, font);
                       if (real_font)
                         {
                            fonts = eina_list_sorted_insert(fonts, sort_cb, eina_stringshare_add(real_font));
                            eina_stringshare_del(real_font);
                         }
                       eina_stringshare_del(font);
                    }
               }
             if (EDJE_PART_TYPE_TEXTBLOCK == type)
               {
                  style = edje_edit_state_text_style_get(obj, part, pstate, pvalue);
                  /* sometimes I hate the edje_edit.  */
                  if (style)
                    {
                       style_fonts = _style_font_get(obj, style);
                       if (!style_fonts)
                         {
                            fonts = eina_list_sorted_merge(fonts, style_fonts, sort_cb);
                         }
                       eina_stringshare_del(style);
                       edje_edit_string_list_free(style_fonts);
                    }
               }
             eina_stringshare_del(pstate);
          }
     }
   edje_edit_string_list_free(parts);

   return _strings_list_duplicates_del(fonts);
}
