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

#include "main_window.h"
#include "preference.h"

static void
_add_part_dialog(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   part_dialog_add(ap);
}

static void
_on_ws_part_unselect(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   App_Data *ap = (App_Data *)data;
   const char *part = (const char *)event_info;

   if (!part) return;

   ui_widget_list_part_selected_set(ui_block_widget_list_get(ap), part, false);
   ui_states_list_data_unset(ui_block_state_list_get(ap));
   ui_property_part_unset(ui_block_property_get(ap));
   /*TODO: in future it will be moved to block api. */
   elm_object_signal_emit(ap->block.bottom_left, "title,content,hide", "eflete");
}

static void
_del_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Style *style = ap->project->current_style;
   Eina_List *programs = NULL;
   Eina_List *l = NULL;
   Evas_Object *gl_signals;
   char *program_name = NULL;
   char *part_name = NULL;
   if (!style) return;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   if (!part)
     {
        NOTIFY_INFO(3, _("No part selected"));
        return;
     }
   part_name = strdup(part->name);
   /* In case when deleting part which is dragable area for another part,
      reloading style into liveview crash application */
   live_view_widget_style_unset(ap->live_view);
   history_diff_add(style->obj, PART_TARGET, DEL, part_name);
   if (workspace_edit_object_part_del(ap->workspace, part->name))
     ui_widget_list_selected_part_del(ui_block_widget_list_get(ap), style);
   style->isModify = true;


   /* If deleted all parts in style, also should deleted all programs*/
   if (!style->parts)
     {
        programs = edje_edit_programs_list_get(style->obj);
        EINA_LIST_FOREACH(programs, l, program_name)
          {
             edje_edit_program_del(style->obj, program_name);
          }
        edje_edit_string_list_free(programs);

        gl_signals = ui_block_signal_list_get(ap);
        ui_signal_list_data_unset(gl_signals);
        _on_ws_part_unselect(ap, ap->workspace, part_name);
        workspace_highlight_unset(ap->workspace);
     }
   else
     {
        live_view_widget_style_set(ap->live_view, ap->project, style);
     }
   free(part_name);
}

static void
_above_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Style *style = ap->project->current_style;
   if (!ui_widget_list_selected_part_above(ui_block_widget_list_get(ap), style))
      return;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   history_diff_add(style->obj, PART_TARGET, RESTACK, part->name);
   if ((!part) || (!workspace_edit_object_part_above(ap->workspace, part->name)))
     {
        NOTIFY_ERROR(_("Internal edje error occurred on part move"));
        ui_widget_list_selected_part_below(ui_block_widget_list_get(ap), style);
     }
   style->isModify = true;
   live_view_widget_style_set(ap->live_view, ap->project, style);
}

static void
_below_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Style *style = ap->project->current_style;
   if (!ui_widget_list_selected_part_below(ui_block_widget_list_get(ap), style))
      return;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   history_diff_add(style->obj, PART_TARGET, RESTACK, part->name);
   if ((!part) || (!workspace_edit_object_part_below(ap->workspace, part->name)))
     {
        NOTIFY_ERROR(_("Internal edje error occurred on part move"));
        ui_widget_list_selected_part_above(ui_block_widget_list_get(ap), style);
     }
   style->isModify = true;
   live_view_widget_style_set(ap->live_view, ap->project, style);
}

static void
_restack_part_above(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info)
{
   App_Data *ap = (App_Data *)data;
   Part *rel = (Part *)event_info;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   Style *style = ap->project->current_style;
   Eina_Inlist *tmp_list = NULL, *tmp_prev = NULL;

   if ((!part) || (!style)) return;
   history_diff_add(style->obj, PART_TARGET, RESTACK, part->name);
   workspace_edit_object_part_restack(ap->workspace, part->name, rel->name, false);
   style->isModify = true;
   live_view_widget_style_set(ap->live_view, ap->project, style);

   tmp_list = eina_inlist_find(style->parts, EINA_INLIST_GET(part));
   tmp_prev = eina_inlist_find(style->parts, EINA_INLIST_GET(rel));
   if (!tmp_list) return;

   style->parts = eina_inlist_remove(style->parts, tmp_list);
   style->parts = eina_inlist_prepend_relative(style->parts, tmp_list, tmp_prev);
}

static void
_restack_part_below(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info)
{
   App_Data *ap = (App_Data *)data;
   Part *rel = (Part *)event_info;
   Style *style = ap->project->current_style;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   Eina_Inlist *tmp_list = NULL, *tmp_prev = NULL;

   if ((!part) || (!style)) return;
   history_diff_add(style->obj, PART_TARGET, RESTACK, part->name);
   workspace_edit_object_part_restack(ap->workspace, part->name, rel->name, true);
   style->isModify = true;
   live_view_widget_style_set(ap->live_view, ap->project, style);

   tmp_list = eina_inlist_find(style->parts, EINA_INLIST_GET(part));
   tmp_prev = eina_inlist_find(style->parts, EINA_INLIST_GET(rel));
   if (!tmp_list) return;

   style->parts = eina_inlist_remove(style->parts, tmp_list);
   style->parts = eina_inlist_append_relative(style->parts, tmp_list, tmp_prev);
}

static void
_show_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   App_Data *ap = (App_Data *)data;
   const char *part_name = (const char *)event_info;
   if (!workspace_edit_object_visible_set(ap->workspace, part_name, true))
     ERR("Can't show groupedit part %s!", part_name);
}

static void
_live_view_update(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   live_view_theme_update(ap->live_view, ap->project);
}

static void
_signal_select(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   App_Data *ap = (App_Data *)data;
   Signal *sig = (Signal *)event_info;
   Part *part = NULL;
   Evas_Object *state_list = NULL;
   Evas_Object *prop_view = NULL;
   evas_object_freeze_events_set(sig->style->obj, false);
   wm_style_state_parts_reset(sig->style);
   edje_edit_program_run(sig->style->obj, sig->program);
   wm_style_current_state_parts_update(sig->style);

   prop_view = ui_block_property_get(ap);
   state_list = ui_block_state_list_get(ap);
   part = ui_states_list_part_get(state_list);
   if (part)
     {
        ui_states_list_data_set(state_list, sig->style, part);
        ui_property_state_set(prop_view, part);
        workspace_edit_object_part_state_set(ap->workspace, part);
     }
   else
     workspace_edit_object_recalc(ap->workspace);
   evas_object_freeze_events_set(sig->style->obj, true);
}


static void
_hide_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   App_Data *ap = (App_Data *)data;
   const char *part_name = (const char *)event_info;
   if (!workspace_edit_object_visible_set(ap->workspace, part_name, false))
     ERR("Can't hide groupedit part %s!", part_name);
}

static void
_add_state_dialog(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   state_dialog_state_add(ap);
}

static void
_del_state_dialog(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   state_dialog_state_del(ap);
}

static void
_del_layout(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   ui_group_delete(data, LAYOUT);
}

static Evas_Object *
_widgetlist_current_genlist_get(App_Data *ap, Type group_type)
{
   Evas_Object *tabs = NULL;
   const Evas_Object *nf = NULL;
   Elm_Object_Item *nf_item = NULL;
   Ewe_Tabs_Item *current_tab_item = NULL;

   if (!ap)
     return false;

   tabs = ui_block_widget_list_get(ap);
   current_tab_item = ewe_tabs_active_item_get(tabs);
   nf = ewe_tabs_item_content_get(tabs, current_tab_item);

   if ((group_type == STYLE) || (group_type == CLASS))
      nf_item = elm_naviframe_top_item_get(nf);
   else
      nf_item = elm_naviframe_bottom_item_get(nf);

   return elm_object_item_part_content_get(nf_item, NULL);
}

static void
_add_layout_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Evas_Object *widget_list = _widgetlist_current_genlist_get(data, LAYOUT);
   Eina_Bool nameExist = true;
   App_Data *ap = (App_Data *)data;
   Widget *widget = NULL;
   Class *class = NULL;
   Style *layout = NULL;
   unsigned int i = 0;
   const char *name = NULL;

   layout = EINA_INLIST_CONTAINER_GET(ap->project->layouts, Style);
   if (!layout)
     {
        widget = EINA_INLIST_CONTAINER_GET(ap->project->widgets, Widget);
        if (widget)
          {
             class = EINA_INLIST_CONTAINER_GET(widget->classes, Class);
             if (class)
               {
                  layout = EINA_INLIST_CONTAINER_GET(class->styles, Style);
                  if (!layout)
                    {
                       NOTIFY_INFO(3, _("Failed create new layout."));
                       ERR("Failed create new layout: edje edit object not find");
                       return;
                    }
               }
             else return;
          }
        else return;
     }
   for (i = 0; i < 999; i++)
     {
        name = eina_stringshare_printf("new/layout/%d", i);
        if (!edje_edit_group_exist(layout->obj, name))
          {
             nameExist = false;
             break;
          }
     }

   if (nameExist)
     {
        NOTIFY_INFO(3, _("Failed create new layout."));
        ERR("Failed create new layout: all avalaible names is exists");
        eina_stringshare_del(name);
        return;
     }

   if (!edje_edit_group_add(layout->obj, name))
     {
        NOTIFY_INFO(3, _("Failed create new layout."));
        ERR("Failed create new layout: edje edit group add fail");
        eina_stringshare_del(name);
        return;
     }

   edje_edit_save_all(layout->obj);
   layout = wm_style_add(name, name, LAYOUT, NULL);
   layout->isModify = true;
   ap->project->layouts = eina_inlist_append(ap->project->layouts,
                                             EINA_INLIST_GET(layout));

   wm_style_data_load(layout, evas_object_evas_get(widget_list),
                      ap->project->dev);
   ui_widget_list_layouts_reload(widget_list, ap->project);
   eina_stringshare_del(name);
   return;
}

static void
_del_style(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_group_delete(ap, STYLE);
}

static void
_add_style_dailog(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   style_dialog_add(ap);
}

static void
_part_name_change(void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
   Part *part = (Part*)event_info;
   App_Data *ap = (App_Data *)data;

   ui_widget_list_part_update(ui_block_widget_list_get(ap), part->name);
   live_view_widget_style_unset(ap->live_view);
   live_view_widget_style_set(ap->live_view, ap->project, ap->project->current_style);
   evas_object_smart_callback_call(ui_block_widget_list_get(ap), "wl,part,select", part);
}

static void
_property_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *prop = ui_block_property_get(ap);
   if (!prop)
     {
        ERR("Property view is missing!");
        return;
     }
   Part *part = (Part *)event_info;
   ui_property_state_set(prop, part);
}

static void
_on_ws_part_select(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   App_Data *ap = (App_Data *)data;
   const char *part = (const char *)event_info;
   if (part)
     ui_widget_list_part_selected_set(ui_block_widget_list_get(ap), part, true);
}

Widget *
ui_widget_from_ap_get(App_Data *ap)
{
   Widget *widget = NULL;
   Evas_Object *gl_widget = NULL;
   Elm_Object_Item *eoi = NULL;

   gl_widget = _widgetlist_current_genlist_get(ap, WIDGET);
   eoi = elm_genlist_selected_item_get(gl_widget);
   if (!eoi)
     {
         NOTIFY_ERROR(_("Couldn't add new state/class without opened widget"));
         return NULL;
     }
   widget = elm_object_item_data_get(eoi);
   return widget;
}

Class *
ui_class_from_ap_get(App_Data *ap)
{
   Evas_Object *gl_class = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *parent_eoi = NULL;

   gl_class = _widgetlist_current_genlist_get(ap, CLASS);
   eoi = elm_genlist_selected_item_get(gl_class);
   if (!eoi)
     {
        eoi = elm_genlist_first_item_get(gl_class);
        if (!eoi) return NULL;
     }
   parent_eoi = elm_genlist_item_parent_get(eoi);
   if(parent_eoi) eoi = parent_eoi;
   return elm_object_item_data_get(eoi);
}


void
ui_part_back(App_Data *ap)
{
   if (!ap) return;

   Evas_Object *wl_list, *groupedit, *st_list, *history_list;

   wl_list = ui_block_widget_list_get(ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,add", _add_part_dialog, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,del", _del_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,above", _above_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,below", _below_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,show", _show_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,hide", _hide_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,moved,up",
                                       _restack_part_above, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,moved,down",
                                       _restack_part_below, ap);
   history_list = ui_block_history_get(ap);
   elm_genlist_clear(history_list);

   groupedit = ws_groupedit_get(ap->workspace);
   evas_object_smart_callback_add(groupedit, "object,area,changed", _live_view_update, ap);
   evas_object_smart_callback_del_full(ap->workspace, "part,name,changed",
                                       _part_name_change, ap);

   workspace_edit_object_unset(ap->workspace);
   st_list = ui_block_state_list_get(ap);
   ui_states_list_data_unset(st_list);
   evas_object_smart_callback_del_full(st_list, "sl,signal,select", _signal_select, ap);
   ui_signal_list_data_unset(ui_block_signal_list_get(ap));
   ui_property_style_unset(ui_block_property_get(ap));
   ui_block_content_visible(ap->block.right_bottom, false);
   /*TODO: in future it will be moved to block api. */
   elm_object_signal_emit(ap->block.bottom_left, "title,content,hide", "eflete");
   live_view_widget_style_unset(ap->live_view);

   ui_menu_style_options_disabled_set(ap->menu_hash, true);

   evas_object_smart_callback_del_full(ap->workspace, "ws,part,selected",
                                       _on_ws_part_select, ap);
   evas_object_smart_callback_del_full(ap->workspace, "ws,part,unselected",
                                       _on_ws_part_unselect, ap);
   evas_object_smart_callback_del_full(ap->workspace, "part,changed", _property_change, ap);
   workspace_highlight_unset(ap->workspace);
}

/**
  * may be usable for future API.
  */
void
ui_style_back(App_Data *ap __UNUSED__)
{
}

void
ui_state_select(App_Data *ap,
                Evas_Object *obj,
                Eina_Stringshare *state)
{
   Part *part = NULL;
   Evas_Object *prop_view;

   if ((!ap) || (!obj))
     {
        ERR("App Data or State list is missing!");
        return;
     }

   prop_view = ui_block_property_get(ap);
   part = ui_states_list_part_get(obj);
   wm_part_current_state_set(part, state);
   ui_property_state_set(prop_view, part);
   workspace_edit_object_part_state_set(ap->workspace, part);
}

Evas_Object *
ui_part_select(App_Data *ap, Part* part)
{
   Evas_Object *prop;
   Evas_Object *gl_states;

   if ((!ap) || (!part))
     {
        ERR("App Data or part is missing!");
        return NULL;
     }
   prop = ui_block_property_get(ap);
   if (!prop)
     {
        ERR("Property view is missing!");
        return NULL;
     }

   ui_property_part_set(prop, part);

   gl_states = ui_states_list_add(ap->block.bottom_left);
   ui_states_list_data_set(gl_states, ap->project->current_style, part);
   ui_block_state_list_set(ap, gl_states);
   evas_object_smart_callback_del_full(gl_states, "stl,state,add", _add_state_dialog, ap);
   evas_object_smart_callback_add(gl_states, "stl,state,add", _add_state_dialog, ap);
   evas_object_smart_callback_del_full(gl_states, "stl,state,del", _del_state_dialog, ap);
   evas_object_smart_callback_add(gl_states, "stl,state,del", _del_state_dialog, ap);

   evas_object_show(gl_states);

   evas_object_smart_callback_del_full(ap->workspace, "part,name,changed",
                                       _part_name_change, ap);
   evas_object_smart_callback_add(ap->workspace, "part,name,changed",
                                  _part_name_change, ap);

   ui_property_state_set(prop, part);
   workspace_edit_object_part_state_set(ap->workspace, part);
   workspace_highlight_unset(ap->workspace);
   workspace_highlight_set(ap->workspace, part);
   evas_object_smart_callback_del_full(ap->workspace, "part,changed", _property_change, ap);
   evas_object_smart_callback_add(ap->workspace, "part,changed", _property_change, ap);

   return gl_states;
}

/* FIXME: rename to style_clicked */
Eina_Bool
ui_style_clicked(App_Data *ap, Style *style)
{
   Evas_Object *wl_list = NULL;
   Evas_Object *gl_signals = NULL;
   Evas_Object *prop = NULL;
   Evas_Object *groupedit = NULL;
   Evas_Object *history_list = NULL;
   Eina_List *signals = NULL;
   Style *_style = NULL, *_alias_style = NULL;

   if ((!ap) || (!ap->project) || (!style))
     {
        ERR("App Data or style is missing!");
        return false;
     }

   _alias_style = style;
   _style = style;
   if (_alias_style->isAlias) _style = _alias_style->main_group;

   wl_list = ui_block_widget_list_get(ap);
   evas_object_smart_callback_add(wl_list, "wl,part,add", _add_part_dialog, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,del", _del_part, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,above", _above_part, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,below", _below_part, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,show", _show_part, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,hide", _hide_part, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,moved,up",
                                  _restack_part_above, ap);
   evas_object_smart_callback_add(wl_list, "wl,part,moved,down",
                                  _restack_part_below, ap);

   /* Get signals list of a styles and show them */
   gl_signals = ui_signal_list_add(ap->block.left_bottom);
   ui_signal_list_data_set(gl_signals, _style);
   wm_program_signals_list_free(signals);
   ui_block_signal_list_set(ap, gl_signals);

   evas_object_smart_callback_add(gl_signals, "sl,signal,select", _signal_select, ap);

   workspace_edit_object_set(ap->workspace, _style, ap->project->dev);
   evas_object_smart_callback_add(ap->workspace, "ws,part,selected",
                                  _on_ws_part_select, ap);
   evas_object_smart_callback_add(ap->workspace, "ws,part,unselected",
                                  _on_ws_part_unselect, ap);
   groupedit = ws_groupedit_get(ap->workspace);
   evas_object_smart_callback_add(groupedit, "object,area,changed", _live_view_update, ap);

   /* style properties */
   prop = ui_block_property_get(ap);
   if (!prop)
     {
        prop = ui_property_add(ap->win);
        ui_block_property_set(ap, prop);
     }
   else
     ui_block_content_visible(ap->block.right_bottom, true);

   ui_property_style_set(prop, _alias_style, ap->workspace);
   evas_object_show(prop);
   ap->project->current_style = _style;

   history_list = history_genlist_get(ap->history, ap->block.right_top);
   history_module_add(_style->obj);
   ui_block_history_set(ap, history_list);

   live_view_widget_style_set(ap->live_view, ap->project, _style);
   ui_menu_style_options_disabled_set(ap->menu_hash, false);

   return true;
}

Eina_Bool
blocks_show(App_Data *ap)
{
   Evas_Object *wd_list;

   wd_list = ui_widget_list_add(ap->block.left_top);
   ui_widget_list_title_set(wd_list, ap->project->name);
   ui_widget_list_data_set(wd_list, ap->project);
   ui_block_widget_list_set(ap, wd_list);
   add_callbacks_wd(wd_list, ap);
   ui_panes_show(ap);

   ui_menu_base_disabled_set(ap->menu_hash, false);
   ui_menu_disable_set(ap->menu_hash, _("Save project"), true);
   ui_menu_disable_set(ap->menu_hash, _("Close project"), false);
   ui_menu_disable_set(ap->menu_hash, _("Separate"), true);
   ui_menu_disable_set(ap->menu_hash, _("Show/Hide object area"), true);

   code_edit_mode_switch(ap, false);

   return true;
}

Eina_Bool
blocks_hide(App_Data *ap)
{
   history_clear(ap->history);
   return ui_panes_hide(ap);
}

Eina_Bool
blocks_data_unset(App_Data *ap)
{
   ui_property_style_unset(ui_block_property_get(ap));
   ui_property_style_unset(ui_block_property_get(ap));
   ui_signal_list_data_unset(ui_block_signal_list_get(ap));
   workspace_edit_object_unset(ap->workspace);
   workspace_highlight_unset(ap->workspace);
   live_view_widget_style_unset(ap->live_view);

   return true;
}

static Eina_Bool
_eflete_filter(const char *path,
               Eina_Bool dir,
               void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".pro"))
     return true;
   return false;
}

static void
_fs_close(void *data __UNUSED__,
          Evas_Object *obj,
          void *event_info __UNUSED__)
{
   evas_object_del(obj);
}

static void
_on_open_done(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info)
{
   Evas_Object *win;
   const char *selected;
   App_Data *ap;

   win = (Evas_Object *)data;
   selected = (const char *)event_info;

   if (!selected) _fs_close(NULL, win, NULL);

   ap = app_data_get();
   ap->project = pm_project_open(selected);
   if (!ap->project) return;

   blocks_data_unset(ap);
   blocks_show(ap);

   evas_object_del(win);
   NOTIFY_INFO(3, _("Project '%s' is opened."), ap->project->name);
   STATUSBAR_PROJECT_PATH(ap, eet_file_get(ap->project->pro));
}

void
project_open(void)
{
   Evas_Object *win, *fs, *bg;
   App_Data *ap;

   ap = app_data_get();
   if ((ap->project) && (!project_close_request(ap,
                              _("You want to open a project, but now you have<br/>"
                                "opened project. If you dont save opened project<br/>"
                                "all your changes will be lost!"))))
     return;


   MODAL_WINDOW_ADD(win, main_window_get(), _("Select a project file"), _fs_close, NULL);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);
   FILESELECTOR_ADD(fs, win, _on_open_done, win);
   elm_fileselector_custom_filter_append(fs, _eflete_filter, NULL, "Eflete Files");
   elm_win_resize_object_add(win, fs);
}

/****************************** Project save **********************************/

static Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string)
{
   App_Data *ap;

   ap = (App_Data *)data;
   elm_object_text_set(ap->splash, progress_string);

   return true;
}

static void
_progress_end(void *data, PM_Project_Result result)
{
   App_Data *ap;

   ap = (App_Data *)data;
   switch (result)
     {
      case PM_PROJECT_ERROR:
        {
           NOTIFY_INFO(3, _("Error: project not saved."));
           break;
        }
      case PM_PROJECT_CANCEL:
        {
           NOTIFY_INFO(3, _("Saving canceled."));
           break;
        }
      case PM_PROJECT_SUCCESS:
        {
           ap->project->changed = false;
           break;
        }
      case PM_PROJECT_LAST: break;
     }

   splash_del(ap->splash);
   ap->splash = NULL;
}

static Eina_Bool
_setup_save_splash(void *data)
{
   App_Data *ap;
   Project_Thread *thread;

   ap = (App_Data *)data;
   thread = pm_project_save(ap->project,
                            _progress_print,
                            _progress_end,
                            data);
   if (!thread) return false;

   return true;
}

static Eina_Bool
_teardown_save_splash(void *data __UNUSED__)
{
   return true;
}

void
project_save(void)
{
   App_Data *ap;

   ap = app_data_get();
   if (!ap->project) return;
   if (!ap->project->changed) return;
   if (ap->splash) return;

   ap->splash = splash_add(ap->win, _setup_save_splash, _teardown_save_splash, ap);
   evas_object_focus_set(ap->splash, true);
   evas_object_show(ap->splash);
   ui_menu_disable_set(ap->menu_hash, _("Save project"), true);
}

/******************************************************************************/

void
project_changed(void)
{
   App_Data *ap;

   ap = app_data_get();

   if (ap->project->changed) return;
   pm_project_changed(ap->project);
   ui_menu_disable_set(ap->menu_hash, _("Save project"), false);
}

Eina_Bool
new_theme_create(App_Data *ap __UNUSED__)
{
   /*
   if ((!ap) || (!ap->win)) return false;
   if (!ui_close_project_request(ap,
                                 _("You want to create a new theme, but now you have<br/>"
                                   "opened project. If you dont save opened project<br/>"
                                   "all your changes will be lost!")))
     return false;
   if (!ecore_file_cp(EFLETE_EDJ_PATH"template.edj", EFLETE_SWAP_PATH"Untitled.edj"))
     {
        ERR("Couldn't copy theme template to cache");
        return false;
     }

   return _ui_edj_load_internal(ap, EFLETE_SWAP_PATH"Untitled.edj", true);
   */
   return false;
}

/*************************** Close request popup ******************************/
/*TODO: I think, this functionality need move to dialogs */
static void
_discard_cb(void *data,
            Evas_Object *obj __UNUSED__,
            void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   *res = true;
   ecore_main_loop_quit();
}

static void
_cancel_cb(void *data,
           Evas_Object *obj __UNUSED__,
           void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   *res = false;
   ecore_main_loop_quit();
}

static void
_save_cb(void *data,
         Evas_Object *obj __UNUSED__,
         void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   project_save();
   *res = true;
   ecore_main_loop_quit();
}

Eina_Bool
project_close_request(App_Data *ap, const char *msg)
{
   if (!msg)
      msg = _("If you dont save the open project<br/>"
            "all your unsaved changes will be lost!");

   if (!ap->project->changed) return true;
   if (ap->project->close_request) return false;
   ap->project->close_request = true;

   Eina_Bool result = false;
   Evas_Object *btn, *label;
   Eina_Stringshare *title;
   ui_menu_locked_set(ap->menu_hash, true);
   title = eina_stringshare_printf(_("Close project %s"), ap->project->name);
   ap->popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(ap->popup, "eflete");
   elm_object_part_text_set(ap->popup, "title,text", title);
   LABEL_ADD(ap->popup, label, msg);
   elm_object_content_set(ap->popup, label);
   BUTTON_ADD(ap->popup, btn, _("Save"));
   evas_object_smart_callback_add(btn, "clicked", _save_cb, &result);
   elm_object_part_content_set(ap->popup, "button1", btn);
   BUTTON_ADD(ap->popup, btn, _("Don't save"));
   evas_object_smart_callback_add(btn, "clicked", _discard_cb, &result);
   elm_object_part_content_set(ap->popup, "button2", btn);
   BUTTON_ADD(ap->popup, btn, _("Cancel"));
   evas_object_smart_callback_add(btn, "clicked", _cancel_cb, &result);
   elm_object_part_content_set(ap->popup, "button3", btn);
   evas_object_show(ap->popup);
   eina_stringshare_del(title);

   ecore_main_loop_begin();

   ui_menu_locked_set(ap->menu_hash, false);
   ap->project->close_request = false;
   evas_object_del(ap->popup);
   ap->popup = NULL;

   return result;
}

/******************************************************************************/

static Eina_Bool
_selected_layout_delete(Evas_Object *genlist, App_Data *ap)
{
   Elm_Object_Item *item_to_del = elm_genlist_selected_item_get(genlist);
   Style *style = elm_object_item_data_get(item_to_del);;
   Style *style_work = NULL;
   Eina_Inlist *l = NULL;

   if (!item_to_del)
     {
        NOTIFY_INFO(3, _("Select item to be deleted"));
        return false;
     }

   EINA_INLIST_FOREACH_SAFE(ap->project->layouts, l, style_work)
     {
        if (strcmp(style->name, style_work->name))
          break;
     }

   if (!style_work)
     {
        NOTIFY_INFO(3, _("Can not delete last layout"));
        return false;
     }

   evas_object_del(style->obj);
   if (!edje_edit_group_del(style_work->obj, style->full_group_name))
     {
        NOTIFY_INFO(3, _("Failed to delete layout[%s]"),
                    style->name);
     }
   ap->project->layouts = eina_inlist_remove(ap->project->layouts,
                                             EINA_INLIST_GET(style));
   ui_widget_list_layouts_reload(genlist, ap->project);
   return true;
}

static Eina_Bool
_selected_style_delete(Evas_Object *genlist, App_Data *ap)
{
   Widget *widget = ui_widget_from_ap_get(ap);
   Widget *widget_work = NULL;
   Class *class_st = NULL;
   Class *class_work = NULL;
   Style *style = NULL;
   Style *alias_style = NULL;
   Style *style_work = NULL;

   Elm_Object_Item *eoi = elm_genlist_selected_item_get(genlist);
   Eina_Inlist *l = NULL;
   Eina_List *ll = NULL;

   if (!eoi)
     {
        NOTIFY_INFO(3, _("No one style is selected"));
        return false;
     }

   /* Check which type type of node selected: style or class */
   if (elm_genlist_item_parent_get(eoi))
     {
        style = elm_object_item_data_get(eoi);
        class_st = style->parent;
     }
   else
      class_st = elm_object_item_data_get(eoi);

   if (class_st->__type != CLASS) return false;

   /* Search edje edit object, which willn't delete now. This object needed
      for manipulate with group in *.edj file*/
   EINA_INLIST_FOREACH(ap->project->widgets, widget_work)
     {
        if (!strcmp(widget->name, widget_work->name)) continue;
        EINA_INLIST_FOREACH(widget_work->classes, class_work)
          {
             EINA_INLIST_FOREACH(class_work->styles, style_work)
              {
                 if (!style_work->isAlias) goto found;
              }
          }
     }

found:
   if (!style_work)
     {
        ERR("Failed search style object from another class");
        return false;
     }

   if (style) /* Deleting style */
     {
        Evas_Object *st_cur = style->obj;

        if (style->isAlias) st_cur = style->main_group->obj;

        if (!edje_edit_group_exist(st_cur, style->full_group_name))
          {
             NOTIFY_INFO(3, _("Style [%s] don't exist"), style->name);
             return false;
          }
        /* Before delete groups from edj file need free tree of aliasses structures*/
        EINA_LIST_FOREACH(style->aliasses, ll, alias_style)
          {
              if (eina_inlist_count(alias_style->parent->styles) <= 1)
                wm_class_free(alias_style->parent);
              else
                wm_style_free(alias_style);
          }
         /*Delete loaded object for unlock group in edj file*/
        evas_object_del(style->obj);
        if (!edje_edit_group_del(style_work->obj, style->full_group_name))
          {
             NOTIFY_INFO(3, _("Failed to delete style[%s]"), style->name);
             return false;
          }
        if (eina_inlist_count(class_st->styles) <= 1)
          wm_class_free(class_st);
        else
          wm_style_free(style);
     }
   else /* Deleting all styles in class. Work slow! Need patch for edje_edit_group_del */
     {
        EINA_INLIST_FOREACH_SAFE(class_st->styles, l, style)
          {
             evas_object_del(style->obj);
             if (!edje_edit_group_del(style_work->obj, style->full_group_name))
               NOTIFY_INFO(3, _("Failed to delete style[%s] in class [%s]"),
                              style->name, class_st->name);
          }
        wm_class_free(class_st);
     }
   style_work->isModify = true;
   ui_widget_list_class_data_reload(genlist, widget->classes);
   return true;
}

Eina_Bool
ui_group_delete(App_Data *ap, Type group_type)
{
   Evas_Object *gl_groups = NULL;

   gl_groups = _widgetlist_current_genlist_get(ap, group_type);

   if (group_type != LAYOUT)
     return _selected_style_delete(gl_groups, ap);
   else
     return _selected_layout_delete(gl_groups, ap);
   return true;
}

Eina_Bool
register_callbacks(App_Data *ap)
{
   if ((!ap) || (!ap->block.left_top)) return false;

   evas_object_smart_callback_add(ap->block.left_top, "wl,style,add",
                                  _add_style_dailog, ap);
   evas_object_smart_callback_add(ap->block.left_top, "wl,style,del",
                                  _del_style, ap);

   evas_object_smart_callback_add(ap->block.left_top, "wl,layout,add",
                                  _add_layout_cb, ap);
   evas_object_smart_callback_add(ap->block.left_top, "wl,layout,del",
                                  _del_layout, ap);

   return true;
}

static void
_on_state_selected(void *data,
                   Evas_Object *obj,
                   void *event_info)
{
   App_Data *ap = (App_Data *)data;
   Elm_Object_Item *eoit = (Elm_Object_Item *)event_info;
   Eina_Stringshare *state = elm_object_item_data_get(eoit);
   ui_state_select(ap, obj, state);
}

static void
_on_style_clicked(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_data)
{
   App_Data *ap = (App_Data *)data;
   Style *_style = (Style *)event_data;
   ui_style_clicked(ap, _style);
}

static void
_on_part_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_data)
{
   App_Data *ap = (App_Data *)data;
   Part *part = (Part *) event_data;
   Evas_Object *gl_states = ui_part_select(ap, part);

   if (gl_states)
     evas_object_smart_callback_add(gl_states, "stl,state,select", _on_state_selected, ap);
}

static void
_on_part_back(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_data __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_part_back(ap);
}

static void
_on_style_back(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_data __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_style_back(ap);
}

Eina_Bool
add_callbacks_wd(Evas_Object *wd_list, App_Data *ap)
{
   if (!wd_list) return false;

   evas_object_smart_callback_add(wd_list, "wl,style,select", _on_style_clicked, ap);
   evas_object_smart_callback_add(wd_list, "wl,part,select", _on_part_selected, ap);
   evas_object_smart_callback_add(wd_list, "wl,part,back", _on_part_back, ap);
   evas_object_smart_callback_add(wd_list, "wl,style,back", _on_style_back, ap);

   return true;
}

static void
_panes_pos_setup(Evas_Object *panes, double value, Eina_Bool disabled)
{
   if (!panes) return;

   elm_panes_content_left_size_set(panes, value);
   elm_object_disabled_set(panes, disabled);
}

Eina_Bool
code_edit_mode_switch(App_Data *ap, Eina_Bool is_on)
{
   Config *config;
   double center = 0.0,
          center_down = 0.0,
          left = 0.0,
          right_hor = 0.0;

    if (is_on)
      {
         if (!config_panes_sizes_data_update(ap)) return false;
      }
    else
      {
         config = config_get();
         if (!config) return false;

         center = config->panes.center;
         center_down = config->panes.center_down;
         left = config->panes.left;
         right_hor = config->panes.right_hor;
      }

   _panes_pos_setup(ap->panes.center, center, is_on);
   _panes_pos_setup(ap->panes.center_down, center_down, is_on);
   _panes_pos_setup(ap->panes.left, left, is_on);
   _panes_pos_setup(ap->panes.right_hor, right_hor, is_on);
   ui_panes_left_panes_min_size_toggle(ap, !is_on);

   return true;
}
