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

#define FLOAT_NUMBER_REGEX "^[0-9]*(\\.[0-9]+)?$"
#define FLOAT_NUMBER_0_1_REGEX "^(0?(\\.[0-9]+)?|1|1\\.0)?$"
#define FLOAT_NUMBER_0_1_REGEX_2_SYMBOLS "^(0?(\\.[0-9]{,2})?|1|1\\.0)?$"
#define EDJE_NAME_REGEX "^[^\\\"]*$"
#define TONE_FRQ_REGEX "^([2-9][0-9]|[1-9][0-9]{2,3}|1[0-9]{4}|20000)$"

#define STATUSBAR_PROJECT_PATH(AP, TEXT) \
   Evas_Object *label = ewe_statusbar_item_content_get( \
         eina_list_nth(ewe_statusbar_items_list_get(AP->statusbar), 1)); \
   elm_object_text_set(label, TEXT);

#define STATUSBAR_PROJECT_SAVE_TIME_UPDATE(AP) \
{ \
   char date[100]; \
   Ewe_Statusbar_Item *item; \
   Evas_Object *lb; \
   long long tm = ecore_file_mod_time(AP->project->dev); \
   item = eina_list_data_get(ewe_statusbar_items_list_get(AP->statusbar)); \
   lb = ewe_statusbar_item_content_get(item); \
   strftime(date, 100, "%d %b %Y %R", localtime((const time_t *)&tm)); \
   elm_object_text_set(lb, date); \
}

/* Getting first object from project. Needed to access top-level blocks */
#define GET_OBJ(PROJECT, EDJE_OBJECT) \
   Eina_Inlist *_styles, *_classes, *_widgets = NULL; \
   Widget *_widget; \
   Class *_class; \
   Style *_style = NULL; \
   if (PROJECT->current_style) EDJE_OBJECT = PROJECT->current_style->obj; \
   else \
     { \
        _widgets = PROJECT->widgets; \
        if (!_widgets) EDJE_OBJECT = NULL; \
        else\
          { \
              _widget = EINA_INLIST_CONTAINER_GET(_widgets, Widget); \
              _classes = _widget->classes; \
              if (!_classes) EDJE_OBJECT = NULL; \
              else \
                { \
                    _class = EINA_INLIST_CONTAINER_GET(_classes, Class); \
                    _styles = _class->styles; \
                    if (!_styles) EDJE_OBJECT = NULL; \
                    else\
                      { \
                          _style = EINA_INLIST_CONTAINER_GET(_styles, Style); \
                          EDJE_OBJECT = _style->obj; \
                      } \
                } \
          } \
        if ((!EDJE_OBJECT) && (PROJECT->layouts)) \
          { \
            _style = EINA_INLIST_CONTAINER_GET(PROJECT->layouts, Style); \
            EDJE_OBJECT = _style->obj;\
          } \
        if (_style->isAlias) \
          EDJE_OBJECT = _style->main_group->obj; \
     }

#define GET_STYLE(PROJECT, STYLE) \
   Eina_Inlist *_styles, *_classes, *_widgets = NULL; \
   Widget *_widget; \
   Class *_class; \
   Style *_style; \
   if (PROJECT->current_style) STYLE = PROJECT->current_style; \
   else \
   { \
     _widgets = PROJECT->widgets; \
     if (!_widgets) STYLE = NULL; \
     else\
     { \
        _widget = EINA_INLIST_CONTAINER_GET(_widgets, Widget); \
        _classes = _widget->classes; \
        if (!_classes) STYLE = NULL; \
        else \
        { \
           _class = EINA_INLIST_CONTAINER_GET(_classes, Class); \
           _styles = _class->styles; \
           if (!_styles) STYLE = NULL; \
           else \
           { \
              _style = EINA_INLIST_CONTAINER_GET(_styles, Style); \
              STYLE = _style; \
           } \
        } \
     } \
     if ((!STYLE) && (PROJECT->layouts)) \
     { \
        _style = EINA_INLIST_CONTAINER_GET(PROJECT->layouts, Style); \
        STYLE = _style;\
     } \
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
   last_item_found = elm_##_gen##_search_by_text_item_get(obj, start_from, \
                                                          PART_NAME, str, 0); \
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
