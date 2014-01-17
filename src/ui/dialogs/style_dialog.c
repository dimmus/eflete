/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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

#include "style_dialog.h"
#include "common_macro.h"
#include "string_macro.h"

static Evas_Object *entry_class;
static Evas_Object *entry_style;

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static Eina_Bool
_group_template_copy(Group *group, Evas_Object *template_edje_edit,
                     Eina_Stringshare *full_name, Evas *e, Project *project,
                     Group *new_group)
{
   Evas_Object *edje_edit_group = NULL;
   Evas_Object *edje_edit = NULL;
   Part *part = NULL;
   Edje_Part_Type part_type;

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
   Eina_Stringshare *part_name = NULL;
   Eina_Stringshare *program_name = NULL;
   Eina_Stringshare *pr_target_name = NULL;
   Eina_Stringshare *pr_after_name = NULL;
   Eina_Stringshare *state_full = NULL;
   Eina_Stringshare *state_name = NULL;
   char *script_source = NULL;
   char **state_tok = NULL;
   double state_val = 0;
   int r, g, b, a;
   int b_l, b_r, b_t, b_b;

#define STATE_COPY(param) \
   edje_edit_state_##param##_set(edje_edit_group, part_name, \
     state_name, state_val,\
     edje_edit_state_##param##_get(template_edje_edit,\
     part_name, state_name, state_val));

#define PART_COPY(param) \
  edje_edit_part_##param##_set(edje_edit_group, part_name, \
    edje_edit_part_##param##_get(template_edje_edit, part_name));

#define PROGRAM_COPY(param) \
  edje_edit_program_##param##_set(edje_edit_group, program_name,\
    edje_edit_program_##param##_get(template_edje_edit, program_name));

   if (!group) return false;
   edje_edit = group->obj;

   if ((!edje_edit) || (!e))
     {
        CRIT("Invalid group object. group->obj[%p] evas[%p]", edje_edit, e);
        return false;
     }

   if (edje_edit_group_exist(edje_edit, full_name))
     {
        NOTIFY_ERROR("Group [%s] exist", full_name);
        return false;
     }

   if (!edje_edit_group_add(edje_edit, full_name))
     {
        NOTIFY_ERROR("Failed create group [%s]", full_name);
        return false;
     }

   edje_edit_group = edje_edit_object_add(e);
   if (!edje_edit_group)
     {
        ERR("Failed created edje_edit object");
        return false;
     }

   if (!edje_object_file_set(edje_edit_group, project->swapfile, full_name))
     {
        NOTIFY_ERROR("Failed load created group to memory");
        evas_object_del(edje_edit_group);
        return false;
     }
   new_group->obj = edje_edit_group;
   /*
    * Copy group attributes.
    */
   edje_edit_group_min_w_set(edje_edit_group,
                             edje_edit_group_min_w_get(template_edje_edit));
   edje_edit_group_min_h_set(edje_edit_group,
                             edje_edit_group_min_h_get(template_edje_edit));
   edje_edit_group_max_w_set(edje_edit_group,
                             edje_edit_group_max_w_get(template_edje_edit));
   edje_edit_group_max_h_set(edje_edit_group,
                             edje_edit_group_max_h_get(template_edje_edit));


   /*
    * Copy parts with attributes and states from template
    * to work edje edit object.
    */
   list_parts = edje_edit_parts_list_get(template_edje_edit);
   EINA_LIST_FOREACH(list_parts, l_parts, part_name)
     {
        part_type = edje_edit_part_type_get(template_edje_edit, part_name);
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
   /*
    *  Copying all states for parts.
    */
   EINA_LIST_FOREACH(list_parts, l_parts, part_name)
     {

       list_states = edje_edit_part_states_list_get(template_edje_edit, part_name);
       EINA_LIST_FOREACH(list_states, l_states, state_full)
          {
             state_tok = eina_str_split(state_full, " ", 0);
             state_name = eina_stringshare_add(state_tok[0]);
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


             edje_edit_state_color_get(template_edje_edit, part_name, state_name,
                                       state_val, &r, &g, &b, &a);
             edje_edit_state_color_set(edje_edit_group, part_name, state_name,
                                       state_val, r, g, b, a);
             edje_edit_state_color2_get(template_edje_edit, part_name, state_name,
                                       state_val, &r, &g, &b, &a);
             edje_edit_state_color2_set(edje_edit_group, part_name, state_name,
                                       state_val, r, g, b, a);
             edje_edit_state_color3_get(template_edje_edit, part_name, state_name,
                                       state_val, &r, &g, &b, &a);
             edje_edit_state_color3_set(edje_edit_group, part_name, state_name,
                                       state_val, r, g, b, a);
             STATE_COPY(align_x);
             STATE_COPY(align_y);

             STATE_COPY(min_w);
             STATE_COPY(min_h);
             STATE_COPY(max_w);
             STATE_COPY(max_h);

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

             part_type = edje_edit_part_type_get(template_edje_edit, part_name);

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
               }
             if (part_type == EDJE_PART_TYPE_IMAGE)
               {
                  STATE_COPY(image);
                  edje_edit_state_image_border_get(template_edje_edit, part_name,
                     state_name, state_val, &b_l, &b_r, &b_t, &b_b);
                   edje_edit_state_image_border_set(edje_edit_group, part_name,
                     state_name, state_val, b_l, b_r, b_t, b_b);
                  STATE_COPY(image_border_fill);
              }

          }
        part = mem_malloc(sizeof(Part));
        part->__type = PART;
        part->name = eina_stringshare_add(part_name);
        part->type = part_type;
        part->curr_state = edje_edit_part_selected_state_get(
                              edje_edit_group, part->name, &state_val);
        part->curr_state_value = state_val;
        part->show = true;
        new_group->parts = eina_inlist_append(new_group->parts,
                                          EINA_INLIST_GET(part));
   }

   /*
    *  Copying programs that used in group.
    */
  list_programs = edje_edit_programs_list_get(template_edje_edit);
  EINA_LIST_FOREACH(list_programs, l_programs, program_name)
     {
        if (!edje_edit_program_add(edje_edit_group, program_name))
          ERR("Failed add program[%s] to group[%s]", program_name, full_name);
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

        list_pr_targets = edje_edit_program_targets_get(template_edje_edit,
                              program_name);
        EINA_LIST_FOREACH(list_pr_targets, l_pr_targets, pr_target_name)
          {
             if (!edje_edit_program_target_add(edje_edit_group, program_name,
                                          pr_target_name))
               ERR("Failed add target[%s] program[%s] to group[%s]",
                   pr_target_name, program_name, full_name);
          }

        list_pr_afters = edje_edit_program_afters_get(template_edje_edit,
                              program_name);
        EINA_LIST_FOREACH(list_pr_afters, l_pr_afters, pr_after_name)
          {
             if (!edje_edit_program_after_add(edje_edit_group, program_name,
                                          pr_after_name))
               ERR("Failed add after[%s] program[%s] to group[%s]",
                   pr_after_name, program_name, full_name);
          }
        script_source = edje_edit_script_program_get(template_edje_edit,
                              program_name);
        if (!script_source) continue;
        edje_edit_script_program_set(template_edje_edit,
                              program_name, script_source);


     }
     script_source = edje_edit_script_get(template_edje_edit);
     if (script_source)
       edje_edit_script_program_set(template_edje_edit, program_name, script_source);

  return true;
#undef PROGRAM_COPY
#undef STATE_COPY
#undef PART_COPY
}

static Widget *
_widget_from_ap_get(App_Data *ap)
{
   Widget *widget = NULL;
   Evas_Object *gl_widget = NULL;
   Evas_Object *nf = NULL;
   Elm_Object_Item *eoi = NULL;

   nf = ui_block_widget_list_get(ap);
   eoi = elm_naviframe_bottom_item_get(nf);
   gl_widget = elm_object_item_part_content_get(eoi, NULL);
   eoi = elm_genlist_selected_item_get(gl_widget);
   if (!eoi)
     {
         NOTIFY_ERROR("Coud'nt add new state/class to no widget");
         return NULL;
     }
   widget = elm_object_item_data_get(eoi);
   return widget;
}

static void
_reload_styles(App_Data *ap, Eina_Inlist *styles)
{
   Evas_Object *gl_style = NULL;
   Elm_Object_Item *eoi = NULL;

   eoi = elm_naviframe_top_item_get(ui_block_widget_list_get(ap));
   gl_style = elm_object_item_part_content_get(eoi, NULL);

   ui_widget_list_style_data_reload(gl_style, styles);
}

static Style *
_style_from_ap_get(App_Data *ap)
{
   Style *style = NULL;
   Evas_Object *gl_style = NULL;
   Evas_Object *nf = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *parent_eoi = NULL;

   nf = ui_block_widget_list_get(ap);
   eoi = elm_naviframe_top_item_get(nf);
   gl_style = elm_object_item_part_content_get(eoi, NULL);
   eoi = elm_genlist_selected_item_get(gl_style);
   if (!eoi)
     {
        eoi = elm_genlist_first_item_get(gl_style);
        if (!eoi) return NULL;
     }
   parent_eoi = elm_genlist_item_parent_get(eoi);
   if(parent_eoi) eoi = parent_eoi;
   style = elm_object_item_data_get(eoi);
   return style;
}

int
_sort_style_add_cb(const void *data1, const void *data2)
{
   Style *style_1 = NULL;
   Style *style_2 = NULL;
   int cmp = 0;

   if (!data1) return 1;
   if (!data2) return -1;

   style_1 = EINA_INLIST_CONTAINER_GET(data1, Style);
   style_2 = EINA_INLIST_CONTAINER_GET(data2, Style);

   cmp = strcmp(style_1->style_name, style_2->style_name);

   if (cmp < 0) return -1;
   if (cmp > 0) return 1;
   return 0;
}


static void
_on_popup_btn_yes(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Eina_Stringshare *style_name = NULL;
   Eina_Stringshare *class_name = NULL;
   Eina_Stringshare *template_file = NULL;
   Eina_Stringshare *full_name = NULL;

   Style *style = NULL;
   Style *style_in_widget = NULL;
   Style *temporary_style = NULL;
   Style *template_style = NULL;
   Widget *widget = NULL;
   Widget *temporary_wdg = NULL;
   Widget *template_wdg = NULL;
   Group *group = NULL;
   Group *temporary_group = NULL;
   Group *class_in_style = NULL;
   Group *template_group = NULL;

   Eina_Bool style_exist = false;
   Eina_Bool template_exist = false;
   Eina_Inlist *l =NULL;
   Eina_Inlist *template_widgets = NULL;
   Eina_List *groups = NULL;
   Evas *canvas = NULL;

#define STRING_CLEAR\
        eina_stringshare_del(template_file);\
        eina_stringshare_del(style_name);\
        eina_stringshare_del(class_name);\


   widget = _widget_from_ap_get(ap);
   if (!widget) return;

   template_file = eina_stringshare_add(TET_EDJ_PATH"template.edj");
   style_name = eina_stringshare_add(elm_entry_entry_get(entry_style));
   class_name = eina_stringshare_add(elm_entry_entry_get(entry_class));

   if (eina_stringshare_strlen(style_name) <= 0)
     {
        NOTIFY_WARNING("Please type style name");
        STRING_CLEAR;
        return;
     }

   if (eina_stringshare_strlen(class_name) <= 0)
     {
        NOTIFY_WARNING("Please type class name");
        STRING_CLEAR;
        return;
     }

   template_widgets = wm_widget_list_new(template_file);
   EINA_INLIST_FOREACH_SAFE(template_widgets, l, template_wdg)
     {
        if (!strcmp(template_wdg->widget_name, widget->widget_name))
          {
             template_exist = true;
             break;
          }
     }

   if (!template_exist)
     {
        NOTIFY_ERROR("Sorry, there are no templates for [%s]",
                     widget->widget_name);
        evas_object_smart_callback_call(obj, "close,popup", NULL);
        STRING_CLEAR;
        return;
     }

   EINA_INLIST_FOREACH_SAFE(widget->styles, l, style_in_widget)
     {
         if (!strcmp(style_in_widget->style_name, style_name))
           {
               style_exist = true;
               break;
           }
     }

   EINA_INLIST_FOREACH_SAFE(ap->project->widgets, l, temporary_wdg)
     {
        if (!strcmp(temporary_wdg->widget_name, widget->widget_name)) break;
     }

   if (!temporary_wdg)
     {
        STRING_CLEAR;
        return;
     }

   canvas = evas_object_evas_get(ap->workspace);
   if (style_exist)
     {
        EINA_INLIST_FOREACH_SAFE(style_in_widget->groups, l, class_in_style)
          {
             if (!strcmp(class_in_style->group_name, class_name))
               {
                  NOTIFY_WARNING("[%s] alredy exist. Type another class name.",
                                 class_in_style->group_name);
                  STRING_CLEAR;
                  return;
               }
             temporary_group = class_in_style;
          }
        full_name = eina_stringshare_printf("elm/%s/%s/%s",
                       widget->widget_name, class_name, style_name);
        group = wm_group_add(class_name, full_name);
        style_in_widget->groups = eina_inlist_append(style_in_widget->groups,
                                                     EINA_INLIST_GET(group));
     }
   else
     {
        full_name = eina_stringshare_printf("elm/%s/%s/%s",
                       temporary_wdg->widget_name, class_name, style_name);

        temporary_style = EINA_INLIST_CONTAINER_GET(temporary_wdg->styles,
                                                    Style);
        temporary_group = EINA_INLIST_CONTAINER_GET(temporary_style->groups,
                                                    Group);
        groups = eina_list_append(groups, full_name);
        style = wm_style_add(style_name, groups);
        if (!style)
          {
             ERR("Failed create style");
             STRING_CLEAR
             return;
          }

        group = EINA_INLIST_CONTAINER_GET(style->groups, Group);
        temporary_wdg->styles = eina_inlist_sorted_insert(temporary_wdg->styles,
                                   EINA_INLIST_GET(style), _sort_style_add_cb);
     }

   EINA_INLIST_FOREACH_SAFE(template_wdg->styles, l, template_style)
     {
        if (!strcmp(template_style->style_name, "default")) break;
     }

   if (!template_style)
     {
        STRING_CLEAR;
        return;
     }

   EINA_INLIST_FOREACH_SAFE(template_style->groups, l, template_group)
     {
        if (!strcmp(template_group->group_name, "base")) break;
     }

   if (!template_group)
      template_group = EINA_INLIST_CONTAINER_GET(template_style->groups, Group);
   wm_group_data_load(template_group, canvas, template_file);

   if (_group_template_copy(temporary_group, template_group->obj, full_name,
                            canvas, ap->project, group))
     _reload_styles(ap, temporary_wdg->styles);

   if (!wm_widget_list_free(template_widgets) && template_widgets)
     ERR("Failed free template widget list");

   STRING_CLEAR;
   eina_stringshare_del(full_name);
   evas_object_smart_callback_call(obj, "close,popup", NULL);
#undef STRING_CLEAR
}

static void
_popup_close(void *data, Evas_Object *obj __UNUSED__, void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}


void
style_dialog_add(App_Data *ap)
{
   Evas_Object *popup, *box, *button;
   Evas_Object *item_style, *item_class;
   Widget *widget = NULL;
   Style *style = NULL;
   Eina_Stringshare *title = NULL;
   Eina_Stringshare *entry_text = NULL;

   if (!ap) return;
   widget = _widget_from_ap_get(ap);
   if (!widget) return;
   title = eina_stringshare_printf("Add style/class for \"%s\" widget",
                                   widget->widget_name);
   popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(popup, "eflete");
   elm_object_part_text_set(popup, "title,text", title);
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(popup, box, false, false);

   ITEM_ADD(box, item_style, "Style name:")
   style = _style_from_ap_get(ap);
   if (!style) return;
   entry_text = eina_stringshare_add(style->style_name);

   ENTRY_ADD(item_style, entry_style, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_style, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry_style, "guide", "Type a new style name.");
   elm_entry_entry_set(entry_style, entry_text);
   elm_object_part_content_set(item_style, "elm.swallow.content", entry_style);

   ITEM_ADD(box, item_class, "Class name:")
   ENTRY_ADD(item_class, entry_class, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_class, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry_class, "guide", "Type a new class name.");
   elm_object_part_content_set(item_class, "elm.swallow.content", entry_class);

   elm_box_pack_end(box, item_style);
   elm_box_pack_end(box, item_class);
   elm_object_content_set(popup, box);

   BUTTON_ADD(popup, button, "Add");
   evas_object_smart_callback_add(button, "close,popup", _popup_close, popup);
   evas_object_smart_callback_add(button, "pressed", _on_popup_btn_yes, ap);
   elm_object_part_content_set(popup, "button1", button);

   BUTTON_ADD(popup, button, "Cancel");
   evas_object_smart_callback_add(button, "clicked", _popup_close, popup);
   elm_object_part_content_set(popup, "button2", button);

   evas_object_show(popup);
   eina_stringshare_del(title);
   eina_stringshare_del(entry_text);
}
