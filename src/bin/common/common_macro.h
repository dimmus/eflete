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

#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

typedef struct _Uns_List Uns_List;
typedef enum _Action_Type
{
   ACTION_TYPE_ADD,
   ACTION_TYPE_DEL,
   ACTION_TYPE_LAST
} Action_Type;

struct _Uns_List
{
   void *data;
   Action_Type act_type;
};

#define HIGHLIGHT_BG_COLOR 0,0,0,0
#define HIGHLIGHT_COLOR 58,110,155,255
#define OBG_AREA_BG_COLOR 64,64,64,64
#define OBG_AREA_COLOR 0,0,0,255

#ifdef HAVE_ENVENTOR
#define ENVENTOR_CODE_BG_COLOR 20,20,20
#endif

#define true EINA_TRUE
#define false EINA_FALSE
#define RETURN_VOID

#ifdef SHOW_TODO
   #define DO_PRAGMA(x) _Pragma (#x)
   #define TODO(x) DO_PRAGMA(message ("TODO - " #x))
#else
   #define TODO(x)
#endif

#define CRIT_ON_FAIL(CALL) \
do { \
if (!(CALL)) \
{ \
   CRIT("Check ("#CALL ")==true failed"); \
   abort(); \
} \
} while (0)

#define FLOAT_NUMBER_REGEX "^[0-9]*(\\.[0-9]+)?$"
#define FLOAT_NUMBER_0_1_REGEX "^(0?(\\.[0-9]+)?|1|1\\.0)?$"
#define FLOAT_NUMBER_0_1_REGEX_2_SYMBOLS "^(0?(\\.[0-9]{,2})?|1|1\\.0)?$"
#define EDJE_NAME_REGEX "^[^\\\"]*$"
#define TONE_FRQ_REGEX "^([2-9][0-9]|[1-9][0-9]{2,3}|1[0-9]{4}|20000)$"

#define STATUSBAR_PROJECT_PATH(TEXT) \
   elm_layout_text_set(ap.win_layout, "eflete.project.part", TEXT);

#define STATUSBAR_PROJECT_SAVE_TIME_UPDATE() \
{ \
   char date[100]; \
   long long tm = ecore_file_mod_time(ap.project->dev); \
   strftime(date, 100, _("Last saved: %d %b %Y %R"), localtime((const time_t *)&tm)); \
   elm_layout_text_set(ap.win_layout, "eflete.project.time", date); \
}

#define ITEM_SEARCH_FUNC(_gen, _GEN_SCROLL, PART_NAME) \
static void \
_##_gen##_item_search(Evas_Object *obj, \
                      Search_Data *search_data, \
                      Elm_Object_Item *start_from) \
{ \
   Eina_Stringshare *str; \
   Elm_Object_Item *last_item_found; \
 \
   if (elm_entry_is_empty(search_data->search_entry)) \
     { \
        if (search_data->last_item_found) \
          { \
             elm_##_gen##_item_selected_set(search_data->last_item_found, \
                                            false); \
             search_data->last_item_found = NULL; \
          } \
        return; \
     } \
 \
   str = eina_stringshare_printf("*%s*", \
                                 elm_entry_entry_get(search_data->search_entry)); \
 \
   last_item_found = elm_##_gen##_search_by_text_item_get(obj, start_from, PART_NAME, str, 0); \
   eina_stringshare_del(str); \
   if (search_data->last_item_found == last_item_found) return; \
 \
   if (search_data->last_item_found) \
      elm_##_gen##_item_selected_set(search_data->last_item_found, false); \
   if (last_item_found) \
     { \
        elm_##_gen##_item_selected_set(last_item_found, true); \
        elm_##_gen##_item_bring_in(last_item_found, \
                                  _GEN_SCROLL); \
        elm_object_focus_set(search_data->search_entry, true); \
     } \
   search_data->last_item_found = last_item_found; \
}


#endif /* COMMON_MACRO_H */
