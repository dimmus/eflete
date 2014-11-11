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
   strcpy(tmp, arr[3]); \
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
_sort_collection_cb(const void *data1, const void *data2)
{
   const char *str1 = eina_stringshare_add(data1);
   const char *str2 = eina_stringshare_add(data2);

   if (!str1) return 1;
   if (!str2) return -1;

   int cmp = (strcmp(str1, str2));
   eina_stringshare_del(str1);
   eina_stringshare_del(str2);
   return cmp;
}

int
_sort_class_cb(const void *data1, const void *data2)
{
   const char *str1 = eina_stringshare_add(data1);
   const char *str2 = eina_stringshare_add(data2);
   char *data1_class = NULL;
   char *data2_class = NULL;

   if (!str1) return 1;
   if (!str2) return -1;

   WM_CLASS_NAME_GET(data1_class, str1);
   WM_CLASS_NAME_GET(data2_class, str2);

   int cmp = (strcmp(data1_class, data2_class));
   free(data1_class);
   free(data2_class);
   eina_stringshare_del(str1);
   eina_stringshare_del(str2);
   return cmp;
}

static void
_wm_part_free(Part *part)
{
   if (!part) return;

   eina_stringshare_del(part->name);
   eina_stringshare_del(part->curr_state);

   free(part);
   part = NULL;
}

Eina_Bool
wm_part_del(Style *style, Part *part)
{
   Eina_Inlist *tmp_list = NULL;

   if ((!style) || (!part)) return false;

   tmp_list = eina_inlist_find(style->parts, EINA_INLIST_GET(part));
   if (!tmp_list) return false;

   style->parts = eina_inlist_remove(style->parts, tmp_list);

   return true;
}

Eina_Bool
wm_style_current_state_parts_update(Style *style)
{
   Part *part = NULL;
   double val = 0;
   const char *state = NULL;
   if (!style) return false;

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
   if (!style) return false;

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

   if ((!part) || (!state) || (!strcmp(state, ""))) return false;
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

   if ((!style) || (!part) || (!style->obj)) return NULL;
   if (!edje_edit_part_exist(style->obj, part))
     return NULL;

   result = (Part *)mem_calloc(1, sizeof(Part));
   result->__type = PART;

   result->name = eina_stringshare_add(part);
   result->type = edje_edit_part_type_get(style->obj, part);
   result->curr_state = edje_edit_part_selected_state_get(style->obj, part, &value);
   result->curr_state_value = value;
   result->show = EINA_TRUE;

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

   if ((!style) || (!style->obj)) return NULL;

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
   if (!signals) return false;

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
wm_style_data_load(Style *style, Evas *e, const char *edj)
{
   Evas_Object *edje_edit_obj;
   Eina_List *parts_list = NULL, *l = NULL;
   char *part_name = NULL;

   if ((!style) || (!e) || (!edj)) return false;

   edje_edit_obj = edje_edit_object_add(e);

   if (!edje_object_file_set(edje_edit_obj, edj, style->full_group_name))
     {
        evas_object_del(edje_edit_obj);
        return false;
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

   if ((!full_group_name) || (!style_name)) return NULL;
   if ((style_type != LAYOUT) && (style_type != STYLE)) return NULL;
   style_edje = (Style *)mem_calloc(1, sizeof(Style));
   style_edje->name = eina_stringshare_add(style_name);
   style_edje->full_group_name = eina_stringshare_add(full_group_name);
   style_edje->obj = NULL;
   style_edje->parts = NULL;
   style_edje->main_group = NULL;
   style_edje->isModify = false;
   style_edje->isAlias = false;
   style_edje->__type = style_type;
   style_edje->parent = parent;
   style_edje->aliasses = NULL;

   return style_edje;
}

Eina_Bool
wm_style_free(Style *style)
{
   Part *part = NULL;
   Eina_List *alias_node = NULL;
   Eina_Inlist *l = NULL;

   if (!style) return false;

   if (style->parent)
     style->parent->styles = eina_inlist_remove(style->parent->styles,
                                              EINA_INLIST_GET(style));

   if (style->isAlias)
     {
         alias_node = eina_list_data_find_list(style->main_group->aliasses, style);
         if (alias_node)
           style->main_group->aliasses = eina_list_remove_list(
                             style->main_group->aliasses, alias_node);
     }

   EINA_INLIST_FOREACH_SAFE(style->parts, l, part)
     {
        _wm_part_free(part);
     }

   if (style->name)
     eina_stringshare_del(style->name);

   eina_stringshare_del(style->full_group_name);

   if (style->obj) evas_object_del(style->obj);

   free(style);
   style = NULL;

   return true;
}

Eina_Bool
wm_style_copy(Evas_Object *dest_edje, Evas_Object *source_edje,
              Eina_Stringshare *full_name, Eina_Stringshare *dest_file,
              Style *style)
{
   Evas_Object *edje_edit_group = NULL;
   Edje_Part_Type part_type;

   Evas *e = NULL;

   Eina_List *list_parts = NULL;
   Eina_List *list_states = NULL;
   Eina_List *list_programs = NULL;
   Eina_List *list_pr_targets = NULL;
   Eina_List *list_pr_afters = NULL;
   Eina_List *l_parts = NULL;
   Eina_List *l_states = NULL;
   Eina_List *l_programs = NULL;
   Eina_List *l_pr_targets = NULL;
   Eina_List *l_pr_afters = NULL;

   char *part_name = NULL;
   char *program_name = NULL;
   char *pr_target_name = NULL;
   char *pr_after_name = NULL;
   char *state_full = NULL;
   char *state_name = NULL;

   char *script_source = NULL;
   char **state_tok = NULL;
   double state_val = 0;
   int r, g, b, a;
   int b_l, b_r, b_t, b_b;

   if ((!source_edje) || (!full_name) || (!dest_file) || (!style))
     return false;

#define STATE_COPY(param) \
   edje_edit_state_##param##_set(edje_edit_group, part_name, \
     state_name, state_val,\
     edje_edit_state_##param##_get(source_edje,\
     part_name, state_name, state_val));

#define PART_COPY(param) \
  edje_edit_part_##param##_set(edje_edit_group, part_name, \
    edje_edit_part_##param##_get(source_edje, part_name));

#define PROGRAM_COPY(param) \
  edje_edit_program_##param##_set(edje_edit_group, program_name,\
    edje_edit_program_##param##_get(source_edje, program_name));

   if (!dest_edje)
     {
        CRIT("Invalid style object. style->obj[%p]", dest_edje);
        return false;
     }

   e = evas_object_evas_get(dest_edje);
   if (!e)
     {
        CRIT("Failure get evas[%p] from object", e);
        return false;
     }

   if (edje_edit_group_exist(dest_edje, full_name))
     {
        NOTIFY_ERROR(_("Group [%s] exist"), full_name);
        return false;
     }

   if (!edje_edit_group_add(dest_edje, full_name))
     {
        NOTIFY_ERROR(_("Failed create style [%s]"), full_name);
        return false;
     }

   edje_edit_group = edje_edit_object_add(e);
   if (!edje_edit_group)
     {
        ERR("Failed created edje_edit object");
        return false;
     }

   if (!edje_object_file_set(edje_edit_group, dest_file, full_name))
     {
        NOTIFY_ERROR(_("Failed load created style to memory."));
        ERR("\n\tFile: %s\n\tStyle name: %s\n\tEdje edit: %p",
                     dest_file, full_name, edje_edit_group);
        evas_object_del(edje_edit_group);
        return false;
     }
   style->obj = edje_edit_group;
   /* Copy group attributes. */
   edje_edit_group_min_w_set(edje_edit_group,
                             edje_edit_group_min_w_get(source_edje));
   edje_edit_group_min_h_set(edje_edit_group,
                             edje_edit_group_min_h_get(source_edje));
   edje_edit_group_max_w_set(edje_edit_group,
                             edje_edit_group_max_w_get(source_edje));
   edje_edit_group_max_h_set(edje_edit_group,
                             edje_edit_group_max_h_get(source_edje));

   /*
    * Copy parts with attributes and states from template
    * to work edje edit object.
    */
   list_parts = edje_edit_parts_list_get(source_edje);
   EINA_LIST_FOREACH(list_parts, l_parts, part_name)
     {
        part_type = edje_edit_part_type_get(source_edje, part_name);
        if (!edje_edit_part_add(edje_edit_group, part_name, part_type))
          CRIT("Failed copy part[%s] from template", part_name);
        PART_COPY(clip_to);
        PART_COPY(source);
        PART_COPY(effect);
        PART_COPY(mouse_events);
        PART_COPY(repeat_events);
        PART_COPY(ignore_flags);
        PART_COPY(scale);

        PART_COPY(drag_x);
        PART_COPY(drag_y);
        PART_COPY(drag_step_x);
        PART_COPY(drag_step_y);
        PART_COPY(drag_count_x);
        PART_COPY(drag_count_y);
        PART_COPY(drag_confine);
     }
   edje_edit_string_list_free(list_parts);
   /*
    *  Copying all states for parts.
    */
   list_parts = edje_edit_parts_list_get(source_edje);
   EINA_LIST_FOREACH(list_parts, l_parts, part_name)
     {
       list_states = edje_edit_part_states_list_get(source_edje, part_name);
       EINA_LIST_FOREACH(list_states, l_states, state_full)
          {
             state_tok = eina_str_split(state_full, " ", 0);
             state_name = state_tok[0];
             state_val = atof(state_tok[1]);
             edje_edit_state_add(edje_edit_group, part_name, state_name, state_val);

             STATE_COPY(rel1_to_x);
             STATE_COPY(rel1_to_y);
             STATE_COPY(rel2_to_x);
             STATE_COPY(rel2_to_y);

             STATE_COPY(rel1_relative_x);
             STATE_COPY(rel1_relative_y);
             STATE_COPY(rel2_relative_x);
             STATE_COPY(rel2_relative_y);

             STATE_COPY(rel1_offset_x);
             STATE_COPY(rel1_offset_y);
             STATE_COPY(rel2_offset_x);
             STATE_COPY(rel2_offset_y);


             edje_edit_state_color_get(source_edje, part_name, state_name,
                                       state_val, &r, &g, &b, &a);
             edje_edit_state_color_set(edje_edit_group, part_name, state_name,
                                       state_val, r, g, b, a);
             edje_edit_state_color2_get(source_edje, part_name, state_name,
                                       state_val, &r, &g, &b, &a);
             edje_edit_state_color2_set(edje_edit_group, part_name, state_name,
                                       state_val, r, g, b, a);
             edje_edit_state_color3_get(source_edje, part_name, state_name,
                                       state_val, &r, &g, &b, &a);
             edje_edit_state_color3_set(edje_edit_group, part_name, state_name,
                                       state_val, r, g, b, a);
             STATE_COPY(align_x);
             STATE_COPY(align_y);

             STATE_COPY(min_w);
             STATE_COPY(min_h);
             STATE_COPY(max_w);
             STATE_COPY(max_h);
             STATE_COPY(fixed_w);
             STATE_COPY(fixed_h);

             STATE_COPY(aspect_min);
             STATE_COPY(aspect_max);
             STATE_COPY(aspect_pref);

             STATE_COPY(fill_origin_relative_x);
             STATE_COPY(fill_origin_relative_y);
             STATE_COPY(fill_origin_offset_x);
             STATE_COPY(fill_origin_offset_y);
             STATE_COPY(fill_size_relative_x);
             STATE_COPY(fill_size_relative_y);
             STATE_COPY(fill_size_offset_x);
             STATE_COPY(fill_size_offset_y);

             STATE_COPY(visible);
             /* FIXME: if uncommemt it line we have segfault.
                Maybe it issue related with patch to edje edit, with appling
                new colors when setting new color class to the part state.
                NEED CHECK IT, AND FIX */
             //STATE_COPY(color_class);

             part_type = edje_edit_part_type_get(source_edje, part_name);

             if (part_type == EDJE_PART_TYPE_TEXT)
               {
                  STATE_COPY(text);
                  STATE_COPY(font);
                  STATE_COPY(text_size);
                  STATE_COPY(text_align_x);
                  STATE_COPY(text_align_y);
                  STATE_COPY(text_elipsis);
                  STATE_COPY(text_fit_x);
                  STATE_COPY(text_fit_y);
                  STATE_COPY(text_min_x);
                  STATE_COPY(text_min_y);
                  STATE_COPY(text_max_x);
                  STATE_COPY(text_max_y);
               }
             if (part_type == EDJE_PART_TYPE_IMAGE)
               {
                  STATE_COPY(image);
                  edje_edit_state_image_border_get(source_edje, part_name,
                     state_name, state_val, &b_l, &b_r, &b_t, &b_b);
                   edje_edit_state_image_border_set(edje_edit_group, part_name,
                     state_name, state_val, b_l, b_r, b_t, b_b);
                  STATE_COPY(image_border_fill);
              }
          }
      edje_edit_string_list_free(list_states);
     }
  edje_edit_string_list_free(list_parts);
  evas_object_hide(edje_edit_group);
   /*
    *  Copying programs that used in group.
    */
  list_programs = edje_edit_programs_list_get(source_edje);
  EINA_LIST_FOREACH(list_programs, l_programs, program_name)
     {
        if (!edje_edit_program_add(edje_edit_group, program_name))
          ERR("Failed add program[%s] to style[%s]", program_name, full_name);
        PROGRAM_COPY(source);
        PROGRAM_COPY(signal);
        PROGRAM_COPY(in_from);
        PROGRAM_COPY(in_range);
        PROGRAM_COPY(action);
        PROGRAM_COPY(filter_part);
        PROGRAM_COPY(transition_time);
        PROGRAM_COPY(transition);
        PROGRAM_COPY(value);
        PROGRAM_COPY(value2);
        PROGRAM_COPY(state);
        PROGRAM_COPY(state2);
        PROGRAM_COPY(api_description);
        PROGRAM_COPY(api_name);
      }
  edje_edit_string_list_free(list_programs);

  list_programs = edje_edit_programs_list_get(source_edje);
  EINA_LIST_FOREACH(list_programs, l_programs, program_name)
    {
       list_pr_targets = edje_edit_program_targets_get(source_edje,
                              program_name);
        EINA_LIST_FOREACH(list_pr_targets, l_pr_targets, pr_target_name)
          {
             if (!edje_edit_program_target_add(edje_edit_group, program_name,
                                          pr_target_name))
               ERR("Failed add target[%s] program[%s] to style[%s]",
                   pr_target_name, program_name, full_name);
          }
        edje_edit_string_list_free(list_pr_targets);

        list_pr_afters = edje_edit_program_afters_get(source_edje,
                              program_name);
        EINA_LIST_FOREACH(list_pr_afters, l_pr_afters, pr_after_name)
          {
             if (!edje_edit_program_after_add(edje_edit_group, program_name,
                                          pr_after_name))
               ERR("Failed add after[%s] program[%s] to group[%s]",
                   pr_after_name, program_name, full_name);
          }
        edje_edit_string_list_free(list_pr_afters);
        script_source = edje_edit_script_program_get(source_edje,
                              program_name);
        if (!script_source) continue;
        edje_edit_script_program_set(source_edje,
                              program_name, script_source);
     }
  edje_edit_string_list_free(list_programs);

  script_source = edje_edit_script_get(source_edje);
  if (script_source)
    edje_edit_script_program_set(source_edje, program_name, script_source);

  return true;
#undef PROGRAM_COPY
#undef STATE_COPY
#undef PART_COPY
}

Class *
wm_class_add(const char *class_name, Eina_List *styles, Widget *parent)
{
   Style *style_edje = NULL;
   Class *class_edje = NULL;
   Eina_List *l;
   char *style_name, *style_name_full;

   if ((!class_name) || (!styles)) return NULL;

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
     }

   free(style_name_full);
   free(style_name);

   return class_edje;
}

Eina_Bool
wm_class_free(Class *class_st)
{
   Style *style_edje = NULL;
   Eina_Inlist *l = NULL;

   if (!class_st) return false;

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
wm_widget_add(const char *widget_name, Eina_List *styles)
{
   Widget *_widget = NULL;
   Eina_List *l, *l_next;
   Eina_List *widget_groups = NULL;
   Class *class_st = NULL;
   char *class_name = NULL, *class_name_next = NULL;
   char *full_style_name, *style_next;

   if ((!widget_name) || (!styles)) return NULL;

   _widget = (Widget *)mem_calloc(1, sizeof(Widget));
   _widget->name = eina_stringshare_add(widget_name);
   _widget->classes = NULL;
   _widget->__type = WIDGET;

   styles = eina_list_sort(styles,
                           eina_list_count(styles),
                           _sort_class_cb);

   EINA_LIST_FOREACH_SAFE(styles, l, l_next, full_style_name)
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

   if (!widget) return false;

   EINA_INLIST_FOREACH_SAFE(widget->classes, l, class_st)
     {
        wm_class_free(class_st);
     }

   eina_stringshare_del(widget->name);
   free(widget);

   return true;
}

Eina_Inlist *
wm_widget_list_new(const char *file)
{
   Widget *widget;
   Eina_List *collection, *l, *l_next;
   Eina_List *widget_styles = NULL;
   Eina_Inlist *widget_list = NULL;
   char *widget_name = NULL, *widget_name_next = NULL;
   char *group, *group_next;
   const char prefix[] = "elm/";
   Eina_Error error;

   if (!file) return NULL;

   DBG("Start to parse the edje group collection.");
   collection = edje_file_collection_list(file);
   if (!collection)
     {
        error = eina_error_get();
        ERR("%s", eina_error_msg_get(error));
     }

   collection = eina_list_sort(collection,
                               eina_list_count(collection),
                               _sort_collection_cb);

   EINA_LIST_FOREACH_SAFE(collection, l, l_next, group)
     {
        if (eina_str_has_prefix(group, prefix))
          {
             free(widget_name_next);
             WM_WIDGET_NAME_GET(widget_name, group);
             if (l_next)
               {
                  group_next = eina_list_data_get(l_next);
                  WM_WIDGET_NAME_GET(widget_name_next, group_next);
               }
             else
               widget_name_next = &empty;
             widget_styles = eina_list_append(widget_styles, group);

             if ((widget_name) && (widget_name_next) && (strcmp(widget_name, widget_name_next) != 0))
               {
                  widget = wm_widget_add(widget_name, widget_styles);
                  widget_list = eina_inlist_append(widget_list,
                                                   EINA_INLIST_GET(widget));
                  widget_styles = eina_list_free(widget_styles);
                  widget_styles = NULL;
               }
             if (widget_name)
               free(widget_name);
          }
     }
   edje_file_collection_list_free(collection);
   DBG("Parse the edje group collection id finished! The list of widget - created.");
   INFO("The theme consists of styles for %i widgets.", eina_inlist_count(widget_list));

   return widget_list;
}

Eina_Inlist *
wm_widget_list_layouts_load(const char *file)
{
   Style *layout;
   Eina_List *collection, *l;
   Eina_Inlist *layouts = NULL;
   char *group;
   const char prefix[] = "elm/";
   Eina_Error error;

   if (!file) return NULL;

   collection = edje_file_collection_list(file);
   if (!collection)
     {
        error = eina_error_get();
        ERR("%s", eina_error_msg_get(error));
     }

   collection = eina_list_sort(collection,
                               eina_list_count(collection),
                               _sort_collection_cb);

   EINA_LIST_FOREACH(collection, l, group)
     {
        if (!eina_str_has_prefix(group, prefix))
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
wm_widget_list_free(Eina_Inlist *widget_list)
{
   Widget *widget = NULL;

   if (!widget_list) return false;

   while (widget_list)
     {
        widget = EINA_INLIST_CONTAINER_GET(widget_list, Widget);
        widget_list = eina_inlist_remove(widget_list, widget_list);
        wm_widget_free(widget);
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

   if ((!widget_list) || (!style_full_name)) return NULL;

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

   if (!_style) return NULL;

   return _style;
}

Style *
_layout_object_find(Eina_Inlist *layout_list, const char *style_full_name)
{
   Style *_layout = NULL;

   if ((!layout_list) || (!style_full_name)) return NULL;

   EINA_INLIST_FOREACH(layout_list, _layout)
     {
        if (!strcmp(_layout->full_group_name, style_full_name)) break;
     }

   if (!_layout) return NULL;

   return _layout;

}

Eina_Bool
wm_widget_list_objects_load(Eina_Inlist *widget_list,
                            Evas *e,
                            const char *path)
{
   Widget *widget = NULL;
   Class *class_st = NULL;
   Style *style = NULL, *alias = NULL;
   Eina_List *alias_list = NULL, *l = NULL;
   const char *main_name;

   if ((!widget_list) || (!e) || (!path)) return false;

   EINA_INLIST_FOREACH(widget_list, widget)
     {
        EINA_INLIST_FOREACH(widget->classes, class_st)
          {
             EINA_INLIST_FOREACH(class_st->styles, style)
               {
                  wm_style_data_load(style, e, path);
                  if (style->isAlias)
                    alias_list = eina_list_append(alias_list, style);
               }
          }
     }

   EINA_LIST_FOREACH(alias_list, l, alias)
     {
        main_name = edje_edit_group_aliased_get(alias->obj, alias->full_group_name);
        alias->main_group = wm_style_object_find(widget_list, main_name);
        alias->main_group->aliasses = eina_list_append(alias->main_group->aliasses, alias);
        evas_object_del(alias->obj);
        alias->obj = NULL;
     }
   eina_list_free(alias_list);
   return true;
}

Eina_Bool
wm_layouts_list_objects_load(Eina_Inlist *layouts_list,
                            Evas *e,
                            const char *path)
{
   Style *layout = NULL, *alias = NULL;
   Eina_List *alias_list = NULL, *l = NULL;
   const char *main_name;

   if ((!layouts_list) || (!e) || (!path)) return false;

   EINA_INLIST_FOREACH(layouts_list, layout)
     {
         wm_style_data_load(layout, e, path);
         if (layout->isAlias)
           alias_list = eina_list_append(alias_list, layout);
     }

   EINA_LIST_FOREACH(alias_list, l, alias)
     {
        main_name = edje_edit_group_aliased_get(alias->obj, alias->full_group_name);
        alias->main_group = _layout_object_find(layouts_list, main_name);
        if (alias->main_group)
          {
             evas_object_del(alias->obj);
             alias->obj = NULL;
          }
     }
   eina_list_free(alias_list);
   return true;
}

const char *
wm_part_type_get(Edje_Part_Type part_type)
{
   if (part_type > part_types_count) return NULL;
   return part_types[part_type];
}

#undef WM_WIDGET_NAME_GET
#undef WM_CLASS_NAME_GET
#undef WM_STYLE_NAME_GET
