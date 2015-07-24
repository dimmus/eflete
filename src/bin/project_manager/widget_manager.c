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

#include "widget_manager.h"
#include "alloc.h"
#include "string_common.h"

static char **arr;
static char tmp[PATH_MAX];
static int size;
static char empty = '\0';
/**
 * ref http://docs.enlightenment.org/auto/edje/group__Edje__Object__Part.html
 */
static char *part_types[] = {
     "NONE",
     "RECTANGLE",
     "TEXT",
     "IMAGE",
     "SWALLOW",
     "TEXTBLOCK",
     "GRADIENT",
     "GROUP",
     "BOX",
     "TABLE",
     "EXTERNAL",
     "PROXY",
     "SPACER"
};
static unsigned int part_types_count = 12;

/**
 * @def WM_WIDGET_NAME_GET
 *
 * @brief get widget name from full group name string.
 *
 * @param[in] full_group_name
 * @param[out] widget
 *
 * @ingroup WidgetManager
 */
 #define WM_WIDGET_NAME_GET(widget, group) \
   arr = eina_str_split(group, "/", 3); \
   if (!arr[1]) \
     widget = NULL; \
   else \
     widget = strdup(arr[1]); \
  free(arr[0]); \
   free(arr);

/**
 * @def WM_CLASS_NAME_GET
 *
 * @brief get class name from full group name string.
 *
 * @param[in] full_group_name
 * @param[out] class_name
 *
 * @ingroup WidgetManager
 */
#define WM_CLASS_NAME_GET(class_name, full_group_name) \
   arr = eina_str_split(full_group_name, "/", 0); \
   class_name = strdup(arr[2]);\
   free(arr[0]); \
   free(arr);

/**
 * @def WM_STYLE_NAME_GET
 *
 * @brief get style name for spical class from full group name string.
 *
 * @param[in] full_group_name
 * @param[in] class_name
 * @param[out] style_name
 *
 * @ingroup WidgetManager
 */
#define WM_STYLE_NAME_GET(style_name, class_name, full_group_name) \
   arr = eina_str_split(full_group_name, "/", 0); \
   eina_strlcpy(tmp, arr[3], sizeof(tmp)); \
   for (size = 4; arr[size]; size++) \
     { \
        if (strcmp(arr[size], class_name)) \
          { \
             eina_strlcat(tmp, "/", PATH_MAX); \
             eina_strlcat(tmp, arr[size], PATH_MAX); \
          } \
     } \
   style_name = strdup(tmp); \
   free(arr[0]); \
   free(arr);

int
_sort_class_cb(const void *data1, const void *data2)
{
   const char *str1 = data1;
   const char *str2 = data2;
   char *data1_class = NULL;
   char *data2_class = NULL;
   int ret;

   if (!str1) return 1;
   if (!str2) return -1;

   WM_CLASS_NAME_GET(data1_class, str1);
   WM_CLASS_NAME_GET(data2_class, str2);

   ret = (strcmp(data1_class, data2_class));
   free(data1_class);
   free(data2_class);
   return ret;
}

static void
_wm_part_free(Part *part)
{
   assert(part != NULL);

   eina_stringshare_del(part->name);
   eina_stringshare_del(part->curr_state);

   if (part->items)
     edje_edit_string_list_free(part->items);
   free(part);
   part = NULL;
}

Eina_Bool
wm_part_del(Style *style, Part *part)
{
   Eina_Inlist *tmp_list = NULL;

   assert(part != NULL);
   assert(style != NULL);

   tmp_list = eina_inlist_find(style->parts, EINA_INLIST_GET(part));

   assert(tmp_list != NULL);

   style->parts = eina_inlist_remove(style->parts, tmp_list);
   _wm_part_free(part);

   return true;
}

Eina_Bool
wm_style_current_state_parts_update(Style *style)
{
   Part *part = NULL;
   double val = 0;
   const char *state = NULL;

   assert(style != NULL);

   EINA_INLIST_FOREACH(style->parts, part)
     {
        state = edje_edit_part_selected_state_get(style->obj, part->name, &val);
        eina_stringshare_replace(&part->curr_state, state);
        part->curr_state_value = val;
     }
   return true;
}

Eina_Bool
wm_style_state_parts_reset(Style *style)
{
   Part *part = NULL;

   assert(style != NULL);

   EINA_INLIST_FOREACH(style->parts, part)
     {
        eina_stringshare_replace(&part->curr_state, "default");
        part->curr_state_value = 0.0;
        edje_edit_part_selected_state_set(style->obj, part->name,
                                          part->curr_state, part->curr_state_value);
     }
   return true;
}

Eina_Bool
wm_part_current_state_set(Part *part, const char *state)
{
   char **split;

   assert(part != NULL);
   assert(state != NULL);
   assert(strcmp(state, ""));

   split = eina_str_split(state, " ", 2);
   eina_stringshare_del(part->curr_state);
   part->curr_state = eina_stringshare_add(split[0]);
   part->curr_state_value = atof(split[1]);
   free(split[0]);
   free(split);
   return true;
}

Part *
wm_part_add(Style *style, const char *part)
{
   Part *result = NULL;
   double value;

   assert(style != NULL);
   assert(style->obj != NULL);
   assert(part != NULL);
   assert(edje_edit_part_exist(style->obj, part));

   result = (Part *)mem_calloc(1, sizeof(Part));
   result->__type = PART;

   result->name = eina_stringshare_add(part);
   result->type = edje_edit_part_type_get(style->obj, part);
   result->curr_state = edje_edit_part_selected_state_get(style->obj, part, &value);
   result->curr_state_value = value;
   result->show = EINA_TRUE;

   if ((result->type == EDJE_PART_TYPE_TABLE) || (result->type == EDJE_PART_TYPE_BOX))
     result->items = edje_edit_part_items_list_get(style->obj, result->name);

   style->parts = eina_inlist_append(style->parts, EINA_INLIST_GET(result));

   return result;
}

Eina_List *
wm_program_signals_list_get(Style *style)
{
   Eina_List *result = NULL;
   Eina_List *progs, *l;
   Eina_Stringshare *prog_name, *sig_name, *source_name;
   Signal *sig = NULL;

   assert(style != NULL);
   assert(style->obj != NULL);

   progs = edje_edit_programs_list_get(style->obj);
   EINA_LIST_FOREACH(progs, l, prog_name)
     {
        sig_name = edje_edit_program_signal_get(style->obj, prog_name);
        source_name = edje_edit_program_source_get(style->obj, prog_name);
        if (sig_name)
          {
             sig = (Signal *)mem_malloc(sizeof(Signal));
             sig->name = eina_stringshare_add(sig_name);
             if (!source_name)
               sig->source = eina_stringshare_add("");
             else
               sig->source = eina_stringshare_add(source_name);
             sig->program = eina_stringshare_add(prog_name);
             sig->style = style;
             result = eina_list_append(result, sig);
          }
     }
   edje_edit_string_list_free(progs);

  return result;
}

Eina_Bool
wm_program_signals_list_free(Eina_List *signals)
{
   Signal *sig;

   EINA_LIST_FREE(signals, sig)
     {
        eina_stringshare_del(sig->program);
        eina_stringshare_del(sig->source);
        eina_stringshare_del(sig->name);
        sig->style = NULL;
        free(sig);
     }

   eina_list_free(signals);
   signals = NULL;

   return true;
}

Eina_Bool
wm_style_data_load(Style *style, Evas *e, Eina_File *mmap_file)
{
   Evas_Object *edje_edit_obj;
   Eina_List *parts_list = NULL, *l = NULL;
   char *part_name = NULL;

   assert(style != NULL);
   assert(e != NULL);

   if (style->obj)
     {
        eina_file_map_free(mmap_file, style->obj);
        evas_object_del(style->obj);
     }

   edje_edit_obj = edje_edit_object_add(e);

   if (!edje_object_mmap_set(edje_edit_obj, mmap_file, style->full_group_name))
     {
        ERR("Can't set mmap object");
        abort();
     }

   edje_object_freeze(edje_edit_obj);
   evas_object_freeze_events_set(edje_edit_obj, true);
   style->obj = edje_edit_obj;

   if (edje_edit_group_alias_is(edje_edit_obj, style->full_group_name))
     {
        style->isAlias = true;
        return true;
     }

   parts_list = edje_edit_parts_list_get(style->obj);
   EINA_LIST_FOREACH(parts_list, l, part_name)
     {
        wm_part_add(style, part_name);
     }
   edje_edit_string_list_free(parts_list);

   return true;
}

Style *
wm_style_add(const char* style_name, const char* full_group_name,
             Type style_type, Class *parent)
{
   Style *style_edje = NULL;

   assert(full_group_name != NULL);
   assert(style_name != NULL);
   assert((style_type == LAYOUT) ||
          (style_type == STYLE));

   style_edje = (Style *)mem_calloc(1, sizeof(Style));
   style_edje->name = eina_stringshare_add(style_name);
   style_edje->full_group_name = eina_stringshare_add(full_group_name);
   style_edje->obj = NULL;
   style_edje->parts = NULL;
   style_edje->main_group = NULL;
   style_edje->isAlias = false;
   style_edje->__type = style_type;
   style_edje->parent = parent;
   style_edje->aliasses = NULL;

   return style_edje;
}

const char *
wm_style_name_set(Style *style, const char *name)
{
   Eina_Stringshare *new_name;

   assert(style != NULL);
   assert(name != NULL);

   if (style->__type == LAYOUT)
     eina_stringshare_replace(&style->full_group_name, name);
   if (style->__type == STYLE)
     {
        arr = eina_str_split(style->full_group_name, "/", 0);
        eina_strlcpy(tmp, arr[3], sizeof(tmp));
        for (size = 4; arr[size]; size++)
          {
             if (strcmp(arr[size], style->parent->name))
               {
                  eina_strlcat(tmp, "/", PATH_MAX);
                     eina_strlcat(tmp, arr[size], PATH_MAX);
               }
          }
        new_name = eina_stringshare_printf("elm/%s/%s/%s", arr[1], arr[2], arr[3]);
        free(arr[0]); \
        free(arr);

        eina_stringshare_replace(&style->full_group_name, new_name);
        eina_stringshare_del(new_name);
     }
   eina_stringshare_replace(&style->name, name);
   return style->name;
}

Eina_Bool
wm_style_layout_is(Style *style)
{
   assert(style != NULL);

   return style->__type == LAYOUT;
}

Eina_Bool
wm_style_free(Style *style)
{
   Style *aliassed;
   Part *part;
   Eina_List *alias_node, *ll;

   assert(style != NULL);

   if (style->parent)
     style->parent->styles = eina_inlist_remove(style->parent->styles,
                                              EINA_INLIST_GET(style));

   if (style->aliasses)
     {
         EINA_LIST_FOREACH(style->aliasses, ll, aliassed)
           {
              aliassed->main_group = NULL;
           }
         eina_list_free(style->aliasses);
     }

   if (style->isAlias)
     {
        if (style->main_group)
          {
             alias_node = eina_list_data_find_list(style->main_group->aliasses, style);
             if (alias_node)
               style->main_group->aliasses = eina_list_remove_list(style->main_group->aliasses, alias_node);
          }
     }

   EINA_INLIST_FREE(style->parts, part)
     {
        style->parts = eina_inlist_remove(style->parts, EINA_INLIST_GET(part));
        _wm_part_free(part);
     }

   if (style->name)
     eina_stringshare_del(style->name);

   eina_stringshare_del(style->full_group_name);

   if (style->obj)
     evas_object_del(style->obj);

   free(style);
   style = NULL;

   return true;
}

Eina_Bool
wm_style_copy(Evas_Object *dest_edje, Eina_Stringshare *source_full_name,
              Eina_Stringshare *full_name, Eina_Stringshare *dest_file,
              Style *style)
{
   Evas *e = NULL;

   assert(dest_edje != NULL);
   assert(source_full_name != NULL);
   assert(full_name != NULL);
   assert(dest_file != NULL);
   assert(style != NULL);

   e = evas_object_evas_get(dest_edje);

   assert(e != NULL);

   assert(!edje_edit_group_exist(dest_edje, full_name));
   assert(edje_edit_group_copy(dest_edje, source_full_name, full_name));

  return true;
}

Class *
wm_class_add(const char *class_name, Eina_List *styles, Widget *parent)
{
   Style *style_edje = NULL;
   Class *class_edje = NULL;
   Eina_List *l;
   char *style_name, *style_name_full;

   assert(class_name != NULL);
   assert(styles != NULL);

   class_edje = (Class *)mem_calloc(1, sizeof(Class));
   class_edje->name = eina_stringshare_add(class_name);
   class_edje->styles = NULL;
   class_edje->__type = CLASS;
   class_edje->parent = parent;
   EINA_LIST_FOREACH(styles, l, style_name_full)
     {
        WM_STYLE_NAME_GET(style_name, class_edje->name, style_name_full);
        style_edje = wm_style_add(style_name, style_name_full, STYLE, class_edje);
        class_edje->styles = eina_inlist_append(class_edje->styles,
                                                EINA_INLIST_GET(style_edje));
        free(style_name);
     }

   return class_edje;
}

Eina_Bool
wm_class_free(Class *class_st)
{
   Style *style_edje = NULL;
   Eina_Inlist *l = NULL;

   assert(class_st != NULL);

   if (class_st->parent)
     class_st->parent->classes = eina_inlist_remove(
         class_st->parent->classes, EINA_INLIST_GET(class_st));


   EINA_INLIST_FOREACH_SAFE(class_st->styles, l, style_edje)
     {
       wm_style_free(style_edje);
     }

   eina_stringshare_del(class_st->name);
   free(class_st);

   return true;
}

Widget *
wm_widget_add(const char *widget_name, Eina_List **styles)
{
   Widget *_widget = NULL;
   Eina_List *l, *l_next;
   Eina_List *widget_groups = NULL;
   Class *class_st = NULL;
   char *class_name = NULL, *class_name_next = NULL;
   char *full_style_name, *style_next;

   assert(widget_name != NULL);
   assert(styles != NULL);
   assert(*styles != NULL);

   _widget = (Widget *)mem_calloc(1, sizeof(Widget));
   _widget->name = eina_stringshare_add(widget_name);
   _widget->classes = NULL;
   _widget->__type = WIDGET;

   *styles = eina_list_sort(*styles,
                            eina_list_count(*styles),
                            _sort_class_cb);

   EINA_LIST_FOREACH_SAFE(*styles, l, l_next, full_style_name)
     {
        free(class_name_next);
        WM_CLASS_NAME_GET(class_name, full_style_name);
        if (l_next)
          {
             style_next = eina_list_data_get(l_next);
             WM_CLASS_NAME_GET(class_name_next, style_next);
          }
        else
          class_name_next = &empty;

        widget_groups = eina_list_append(widget_groups, full_style_name);

        if (strcmp(class_name, class_name_next) != 0)
          {
             class_st = wm_class_add(class_name, widget_groups, _widget);
             _widget->classes = eina_inlist_append(_widget->classes,
                                                  EINA_INLIST_GET(class_st));
             eina_list_free(widget_groups);
             widget_groups = NULL;
          }
        free(class_name);
     }

   return _widget;
}

Eina_Bool
wm_widget_free(Widget *widget)
{
   Class *class_st = NULL;
   Eina_Inlist *l = NULL;

   assert(widget != NULL);

   EINA_INLIST_FOREACH_SAFE(widget->classes, l, class_st)
     {
        wm_class_free(class_st);
     }

   eina_stringshare_del(widget->name);
   free(widget);

   return true;
}

Eina_Inlist *
wm_widgets_list_new(const char *file)
{
   Widget *widget;
   Eina_List *collection, *l, *l_next;
   Eina_List *widget_styles = NULL;
   Eina_Inlist *widget_list = NULL;
   char *widget_name = NULL, *widget_name_next = NULL;
   char *group, *group_next;
   const char prefix[] = "elm/";
   Eina_Error error;

   assert(file != NULL);

   DBG("Start to parse the edje group collection. From file %s", file);
   collection = edje_file_collection_list(file);
   if (!collection)
     {
        error = eina_error_get();
        ERR("%s", eina_error_msg_get(error));
     }

   collection = eina_list_sort(collection,
                               eina_list_count(collection),
                               sort_cb);

   EINA_LIST_FOREACH_SAFE(collection, l, l_next, group)
     {
        if (eina_str_has_prefix(group, prefix))
          {
             WM_WIDGET_NAME_GET(widget_name, group);
             if (l_next)
               {
                  group_next = eina_list_data_get(l_next);
                  if (eina_str_has_prefix(group_next, prefix))
                    {
                       WM_WIDGET_NAME_GET(widget_name_next, group_next);
                    }
                  else
                    widget_name_next = &empty;
               }
             else
               widget_name_next = &empty;

             widget_styles = eina_list_append(widget_styles, group);

             if ((widget_name) && (widget_name_next) && (strcmp(widget_name, widget_name_next) != 0))
               {
                  widget = wm_widget_add(widget_name, &widget_styles);
                  widget_list = eina_inlist_append(widget_list,
                                                   EINA_INLIST_GET(widget));
                  widget_styles = eina_list_free(widget_styles);
               }
             if (widget_name)
               free(widget_name);

             TODO("change logic here on refactor to make this check unnecessary!!")
             if (widget_name_next != &empty)
               free(widget_name_next);
          }
     }
   edje_file_collection_list_free(collection);
   DBG("Parse the edje group collection id finished! The list of widget - created.");
   INFO("The theme consists of styles for %i widgets.", eina_inlist_count(widget_list));

   return widget_list;
}

Eina_Inlist *
wm_layouts_list_new(const char *file)
{
   Style *layout;
   Eina_List *collection, *l;
   Eina_Inlist *layouts = NULL;
   char *group;
   const char prefix[] = "elm/";
   Eina_Error error;

   assert(file != NULL);

   collection = edje_file_collection_list(file);
   if (!collection)
     {
        error = eina_error_get();
        ERR("%s", eina_error_msg_get(error));
     }

   collection = eina_list_sort(collection,
                               eina_list_count(collection),
                               sort_cb);

   EINA_LIST_FOREACH(collection, l, group)
     {
        if (!eina_str_has_prefix(group, prefix) && strcmp(group, EFLETE_INTERNAL_GROUP_NAME))
          {
             layout = wm_style_add(group, group, LAYOUT, NULL);
             layouts = eina_inlist_append(layouts, EINA_INLIST_GET(layout));
          }
     }
   edje_file_collection_list_free(collection);
   DBG("Parse the edje group collection on layouts id finished!");
   INFO("The file contain styles for %i layouts.", eina_inlist_count(layouts));
   return layouts;
}

Eina_Bool
wm_widgets_list_free(Eina_Inlist *widget_list)
{
   Widget *widget = NULL;

   while (widget_list)
     {
        widget = EINA_INLIST_CONTAINER_GET(widget_list, Widget);
        widget_list = eina_inlist_remove(widget_list, widget_list);
        wm_widget_free(widget);
     }

   return true;
}

Eina_Bool
wm_layouts_list_free(Eina_Inlist *widget_list)
{
   Style *style = NULL;

   while (widget_list)
     {
        style = EINA_INLIST_CONTAINER_GET(widget_list, Style);
        widget_list = eina_inlist_remove(widget_list, widget_list);
        wm_style_free(style);
     }

   return true;
}


Style *
wm_style_object_find(Eina_Inlist *widget_list, const char *style_full_name)
{
   char *widget_name = NULL;
   char *class_name = NULL;
   char *style_name = NULL;
   Widget *_widget = NULL;
   Class *_class = NULL;
   Style *_style = NULL;

   assert(widget_list != NULL);
   assert(style_full_name != NULL);

   WM_WIDGET_NAME_GET(widget_name, style_full_name);
   if (!widget_name)
     return NULL;

   if (widget_name [0] <= 'm')
     EINA_INLIST_FOREACH(widget_list, _widget)
       {
          if (!strcmp(_widget->name, widget_name))
            break;
       }
   else
     EINA_INLIST_REVERSE_FOREACH(widget_list, _widget)
       {
          if (!strcmp(_widget->name, widget_name))
            break;
       }

   if (!_widget)
     {
        free(widget_name);
        return NULL;
     }

   WM_CLASS_NAME_GET(class_name, style_full_name);
   if (class_name [0] <= 'm')
     EINA_INLIST_FOREACH(_widget->classes, _class)
       {
          if (!strcmp(_class->name, class_name))
            break;
       }
   else
     EINA_INLIST_REVERSE_FOREACH(_widget->classes, _class)
       {
          if (!strcmp(_class->name, class_name))
            break;
       }

   if (!_class)
     {
        free(widget_name);
        free(class_name);
        return NULL;
     }

   WM_STYLE_NAME_GET(style_name, class_name, style_full_name)
   if (style_name [0] <= 'm')
     EINA_INLIST_FOREACH(_class->styles, _style)
       {
          if (!strcmp(_style->name, style_name))
            break;
       }
   else
     EINA_INLIST_REVERSE_FOREACH(_class->styles, _style)
       {
          if (!strcmp(_style->name, style_name))
            break;
       }

   free(widget_name);
   free(class_name);
   free(style_name);

   return _style;
}

Style *
_layout_object_find(Eina_Inlist *layout_list, const char *style_full_name)
{
   Style *_layout = NULL;

   assert(layout_list != NULL);
   assert(style_full_name != NULL);

   EINA_INLIST_FOREACH(layout_list, _layout)
     {
        if (!strcmp(_layout->full_group_name, style_full_name)) break;
     }

   return _layout;
}

Eina_Bool
wm_widgets_list_objects_load(Eina_Inlist *widget_list,
                             Evas *e,
                             Eina_File *mmap_file)
{
   Widget *widget = NULL;
   Class *class_st = NULL;
   Style *style = NULL;

   assert(e != NULL);
   assert(mmap_file != NULL);

   EINA_INLIST_FOREACH(widget_list, widget)
     {
        EINA_INLIST_FOREACH(widget->classes, class_st)
          {
             EINA_INLIST_FOREACH(class_st->styles, style)
               {
                  wm_style_data_load(style, e, mmap_file);
               }
          }
     }
   return true;
}

Eina_Bool
wm_widgets_list_objects_del(Eina_Inlist *widget_list)
{
   Widget *widget = NULL;
   Class *class_st = NULL;
   Style *style = NULL;

   assert(widget_list != NULL);

   EINA_INLIST_FOREACH(widget_list, widget)
     {
        EINA_INLIST_FOREACH(widget->classes, class_st)
          {
             EINA_INLIST_FOREACH(class_st->styles, style)
               {
                  evas_object_del(style->obj);
               }
          }
     }

   return true;
}


Eina_Bool
wm_layouts_list_objects_load(Eina_Inlist *layouts_list,
                            Evas *e,
                            Eina_File *mmap_file)
{
   Style *layout = NULL;

   assert(e != NULL);
   assert(mmap_file != NULL);

   EINA_INLIST_FOREACH(layouts_list, layout)
     {
         wm_style_data_load(layout, e, mmap_file);
     }

   return true;
}

const char *
wm_part_type_get(Edje_Part_Type part_type)
{
   assert(part_type <= part_types_count);

   return part_types[part_type];
}

Part *
wm_part_by_name_find(Style *style, Eina_Stringshare *part_name)
{
   Part *data = NULL;

   assert(style != NULL);
   assert(part_name != NULL);

   EINA_INLIST_FOREACH(style->parts, data)
     {
        if (data->name == part_name) return data;
     }
   return NULL;
}

Eina_Bool
wm_style_parts_restack(Style *style, Eina_Stringshare *part_name,
                       Eina_Stringshare *rel_name)
{
   Part *data = NULL;
   Part *part = NULL;
   Part *rel_part = NULL;
   Eina_Inlist *tmp_list = NULL, *tmp_prev = NULL;

   assert(style != NULL);
   assert(part_name != NULL);

   EINA_INLIST_FOREACH(style->parts, data)
     {
        TODO("Check logic here. Seems like we are skipping rel_part if part was found earlier")
        if (data->name == part_name) part = data;
        else if (data->name == rel_name)
         rel_part = data;
        if (part) break;
     }

   assert(part != NULL);

   tmp_list = eina_inlist_find(style->parts, EINA_INLIST_GET(part));

   assert(tmp_list != NULL);

   if (rel_part)
    tmp_prev = eina_inlist_find(style->parts, EINA_INLIST_GET(rel_part));

   style->parts = eina_inlist_remove(style->parts, tmp_list);
   if (!tmp_prev)
      style->parts = eina_inlist_append_relative(style->parts, tmp_list, tmp_prev);
   else
      style->parts = eina_inlist_prepend_relative(style->parts, tmp_list, tmp_prev);

   return true;
}

Eina_Bool
wm_style_data_reload(Style *style, Eina_File *mmap_file)
{
   Eina_List *parts_list = NULL, *l = NULL;
   char *part_name = NULL;
   Part *part = NULL;

   assert(style != NULL);
   assert(style->obj != NULL);
   assert(style->full_group_name != NULL);
   assert(mmap_file != NULL);

   eina_file_map_free(mmap_file, style->obj);
   edje_object_mmap_set(style->obj, mmap_file, style->full_group_name);

   EINA_INLIST_FREE(style->parts, part)
     {
        style->parts = eina_inlist_remove(style->parts, EINA_INLIST_GET(part));
        _wm_part_free(part);
     }

   parts_list = edje_edit_parts_list_get(style->obj);
   EINA_LIST_FOREACH(parts_list, l, part_name)
     {
        wm_part_add(style, part_name);
     }
   edje_edit_string_list_free(parts_list);

   return true;
}

TODO("Add UTC for this function!")
Eina_Bool
wm_styles_build_alias(Eina_Inlist *widgets_list,
                      Eina_Inlist *layouts_list)
{
   Style *layout = NULL;
   Widget *widget = NULL;
   Class *class_st = NULL;
   const char *main_name;

   /* checking layouts for awesomeness */
   if (layouts_list)
     {
        EINA_INLIST_FOREACH(layouts_list, layout)
          {
             if (layout->isAlias)
               {
                  main_name = edje_edit_group_aliased_get(layout->obj, layout->full_group_name);
                  layout->main_group = _layout_object_find(layouts_list, main_name);
                  if ((!layout->main_group) && (widgets_list))
                    layout->main_group = wm_style_object_find(widgets_list, main_name);

                  if (layout->main_group)
                    {
                       layout->main_group->aliasses = eina_list_append(layout->main_group->aliasses, layout);
                       evas_object_del(layout->obj);
                       layout->obj = NULL;
                    }
               }
          }
     }

   /* checking layouts for awesomeness */
   if (widgets_list)
     {
        EINA_INLIST_FOREACH(widgets_list, widget)
          {
             EINA_INLIST_FOREACH(widget->classes, class_st)
               {
                  EINA_INLIST_FOREACH(class_st->styles, layout)
                    {
                       if (layout->isAlias)
                         {
                            main_name = edje_edit_group_aliased_get(layout->obj, layout->full_group_name);
                            layout->main_group = wm_style_object_find(widgets_list, main_name);
                            if ((!layout->main_group) && (layouts_list))
                              layout->main_group = _layout_object_find(layouts_list, main_name);

                            if (layout->main_group)
                              {
                                 layout->main_group->aliasses = eina_list_append(layout->main_group->aliasses, layout);
                                 evas_object_del(layout->obj);
                                 layout->obj = NULL;
                              }
                         }
                    }
               }
          }
     }
   return true;
}

#undef WM_WIDGET_NAME_GET
#undef WM_CLASS_NAME_GET
#undef WM_STYLE_NAME_GET
