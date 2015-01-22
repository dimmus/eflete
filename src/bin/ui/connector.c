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
#ifdef HAVE_ENVENTOR
   #define ENVENTOR_BETA_API_SUPPORT
   #include "Enventor.h"
#endif /* HAVE_ENVENTOR */

#ifdef HAVE_ENVENTOR
#define ENVENTOR_BETA_API_SUPPORT
#include "Enventor.h"
#include "main_window.h"
#endif

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
   project_changed();
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
   if (!part) return;
   history_diff_add(style->obj, PART_TARGET, RESTACK, part->name);
   if (!workspace_edit_object_part_above(ap->workspace, part->name))
     {
        NOTIFY_ERROR(_("Internal edje error occurred on part move"));
        ui_widget_list_selected_part_below(ui_block_widget_list_get(ap), style);
     }
   style->isModify = true;
   live_view_widget_style_set(ap->live_view, ap->project, style);
   project_changed();
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
   if (!part) return;
   history_diff_add(style->obj, PART_TARGET, RESTACK, part->name);
   if (!workspace_edit_object_part_below(ap->workspace, part->name))
     {
        NOTIFY_ERROR(_("Internal edje error occurred on part move"));
        ui_widget_list_selected_part_above(ui_block_widget_list_get(ap), style);
     }
   style->isModify = true;
   live_view_widget_style_set(ap->live_view, ap->project, style);
   project_changed();
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
   project_changed();
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
   project_changed();
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
   App_Data *ap = (App_Data *)data;
   ui_group_delete(ap, LAYOUT);
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
   Evas_Object *widget_list = _widgetlist_current_genlist_get(data, LAYOUT), *eoi;
   Eina_Bool nameExist = true;
   App_Data *ap = (App_Data *)data;
   Widget *widget = NULL;
   Class *class = NULL;
   Style *layout = NULL;
   unsigned int i = 0;
   const char *name = NULL;
   Evas_Object *group_obj;

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
                       NOTIFY_INFO(3, _("Failed to create new layout."));
                       ERR("Failed to create new layout: no edje edit object is found");
                       return;
                    }
               }
             else return;
          }
        else return;
     }

   /* Using aliased group, if the group we've found is alias. */
   group_obj = (layout->isAlias) ? layout->main_group->obj : layout->obj;

   for (i = 0; i < 999; i++)
     {
        name = eina_stringshare_printf("new/layout/%d", i);
        if (!edje_edit_group_exist(group_obj, name))
          {
             nameExist = false;
             break;
          }
     }

   if (nameExist)
     {
        NOTIFY_INFO(3, _("Failed to create new layout."));
        ERR("Failed to create new layout: all avalaible names are existing");
        eina_stringshare_del(name);
        return;
     }

   if (!edje_edit_group_add(group_obj, name))
     {
        NOTIFY_INFO(3, _("Failed to create new layout."));
        ERR("Failed to create new layout: edje edit group add is failed");
        eina_stringshare_del(name);
        return;
     }

   layout = wm_style_add(name, name, LAYOUT, NULL);
   layout->isModify = true;
   ap->project->layouts = eina_inlist_append(ap->project->layouts,
                                             EINA_INLIST_GET(layout));
   wm_style_data_load(layout, evas_object_evas_get(widget_list),
                      ap->project->mmap_file);
   ui_widget_list_layouts_reload(widget_list, ap->project);
   widget_list = _widgetlist_current_genlist_get(ap, LAYOUT);
   eoi = elm_genlist_last_item_get(widget_list);
   elm_genlist_item_selected_set(eoi, true);
   eina_stringshare_del(name);

   project_changed();
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
   evas_object_hide(history_list);

   groupedit = ws_groupedit_get(ap->workspace);
   evas_object_smart_callback_del_full(groupedit, "object,area,changed",
                                       _live_view_update, ap);
   evas_object_smart_callback_del_full(ap->workspace, "part,name,changed",
                                       _part_name_change, ap);

   workspace_edit_object_unset(ap->workspace);
   st_list = ui_block_state_list_get(ap);
   ui_states_list_data_unset(st_list);
   evas_object_smart_callback_del_full(st_list, "sl,signal,select", _signal_select, ap);
   ui_signal_list_data_unset(ui_block_signal_list_get(ap));
   ui_block_content_visible(ap->block.right_bottom, false);
   /*TODO: in future it will be moved to block api. */
   elm_object_signal_emit(ap->block.bottom_left, "title,content,hide", "eflete");
   live_view_widget_style_unset(ap->live_view);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

   evas_object_smart_callback_del_full(ap->workspace, "ws,part,selected",
                                       _on_ws_part_select, ap);
   evas_object_smart_callback_del_full(ap->workspace, "ws,part,unselected",
                                       _on_ws_part_unselect, ap);
   evas_object_smart_callback_del_full(ap->workspace, "part,changed", _property_change, ap);
   workspace_highlight_unset(ap->workspace);
#ifdef HAVE_ENVENTOR
   eina_stringshare_del(ap->project->enventor.file);
   eina_stringshare_del(ap->project->enventor.path);
   ap->project->enventor.file = NULL;
#endif /* HAVE_ENVENTOR */
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

   ui_block_content_visible(ap->block.right_bottom, true);
   ui_property_style_set(prop, _alias_style, ap->workspace);
   evas_object_show(prop);
   ap->project->current_style = _style;

   history_list = history_genlist_get(ap->history, ap->block.right_top);
   history_module_add(_style->obj);
   ui_block_history_set(ap, history_list);

   live_view_widget_style_set(ap->live_view, ap->project, _style);
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_STYLE_ONLY, false);

   return true;
}

static inline void
_widget_list_layouts_tab_activate(App_Data *ap)
{
   if (!eina_inlist_count(ap->project->widgets))
     ui_widget_list_tab_activate(ui_block_widget_list_get(ap), 1);
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

   _widget_list_layouts_tab_activate(ap);

   ui_panes_show(ap);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_BASE, false);
   ui_menu_disable_set(ap->menu, MENU_FILE_SAVE, true);

   code_edit_mode_switch(ap, false);

   return true;
}

static void
_blocks_data_unset(App_Data *ap)
{
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

   ui_block_content_visible(ap->block.right_bottom, false);
   ui_signal_list_data_unset(ui_block_signal_list_get(ap));
   ui_states_list_data_unset(ui_block_state_list_get(ap));
   history_clear(ap->history);
   workspace_edit_object_unset(ap->workspace);
   workspace_highlight_unset(ap->workspace);
   live_view_widget_style_unset(ap->live_view);
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
_fs_close(void *data,
          Evas_Object *obj,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ap->modal_editor--;
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
   ap = app_data_get();

   if (!selected)
     {
        ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
        ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_BASE, true);
        _fs_close(ap, win, NULL);
        return;
     }

   ap->project = pm_project_open(selected);
   if (!ap->project) return;

   wm_widgets_list_objects_load(ap->project->widgets,
                                evas_object_evas_get(ap->win),
                                ap->project->mmap_file);
   wm_layouts_list_objects_load(ap->project->layouts,
                                evas_object_evas_get(ap->win),
                                ap->project->mmap_file);
   blocks_show(ap);

   evas_object_del(win);
   ap->modal_editor--;
   NOTIFY_INFO(3, _("Project '%s' is opened."), ap->project->name);
   STATUSBAR_PROJECT_PATH(ap, eet_file_get(ap->project->pro));
   STATUSBAR_PROJECT_SAVE_TIME_UPDATE(ap);

   _widget_list_layouts_tab_activate(ap);
}

#define PROJECT_CLOSE_MSG _("Do you want to save changes?")

Eina_Bool
project_close(App_Data *ap)
{
   if (ap->project)
     {
        if ((ap->project->changed) && (!project_close_request(ap, PROJECT_CLOSE_MSG)))
          return false;

        STATUSBAR_PROJECT_PATH(ap, _("No project opened"));
        ui_panes_hide(ap);
        _blocks_data_unset(ap);
        pm_project_close(ap->project);
        ap->project = NULL;
     }
   return true;
}

void
project_open(void)
{
   Evas_Object *win, *fs, *bg;
   App_Data *ap;

   ap = app_data_get();

   if (!project_close(ap))
     return;

   ap->modal_editor++;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_BASE, true);
   MODAL_WINDOW_ADD(win, main_window_get(), _("Select a project file"), _fs_close, ap);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);
   FILESELECTOR_ADD(fs, win, _on_open_done, win);
   elm_fileselector_custom_filter_append(fs, _eflete_filter, NULL, "Eflete Files");
   elm_fileselector_mime_types_filter_append(fs, "*", "All Files");
   elm_win_resize_object_add(win, fs);
}

#undef PROJECT_CLOSE_MSG
/****************************** Project save **********************************/

static Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string)
{
   App_Data *ap;

   ap = (App_Data *)data;
   elm_object_part_text_set(ap->splash, "label.info", progress_string);

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

#ifdef HAVE_ENVENTOR
   if (ap->enventor_mode)
     {
        wm_widgets_list_objects_load(ap->project->widgets,
                                     evas_object_evas_get(ap->win),
                                     ap->project->mmap_file);
        pm_project_changed(ap->project);
     }
#endif /* HAVE_ENVENTOR */

   live_view_widget_style_unset(ap->live_view);
   live_view_widget_style_set(ap->live_view, ap->project, ap->project->current_style);
   splash_del(ap->splash);
   ap->splash = NULL;

}

static Eina_Bool
_setup_save_splash(void *data, Splash_Status status __UNUSED__)
{
   App_Data *ap;
   Project_Thread *thread;

   ap = (App_Data *)data;
#ifdef HAVE_ENVENTOR
   char *code, *new_code;
   char *version_ptr, *search_ptr;
   long long f_size;
   size_t code_r;
   FILE *f;
   long int concat_pos = 1;
   char *data_str = "\"version\" \"110\";";
   long int data_len = strlen(data_str);

   if (ap->enventor_mode)
     {
        enventor_object_save(ap->enventor, ap->project->enventor.file);

        f_size = ecore_file_size(ap->project->enventor.file);
        f = fopen(ap->project->enventor.file, "r+");
        if (!f)
          {
             ERR("Failed set the Elementary version support to '%s'",
                 ap->project->enventor.file);
             return false;
          }
        code = mem_calloc(1, f_size);
        code_r = fread(code, 1, f_size, f);
        if (code_r == 0)
          {
             free(code);
             fclose(f);
             return false;
          }
        version_ptr = strstr(code, "\"version\"");
        if (!version_ptr)
          {
             rewind(f);
             fputs("data.item: \"version\" \"110\";\n\n", f);
             fputs(code, f);
          }
        else
          {
             fclose(f);
             f = fopen(ap->project->enventor.file, "w");

             /*Search position where item block ends.*/
             for(search_ptr = version_ptr;
                 ((!search_ptr) || (*search_ptr != ';'));
                 search_ptr++, concat_pos++);

             /*
              * Position of the source code, where it should be merged
              * into temporary buffer.
              */
              concat_pos += (version_ptr - code);

              /*
              * Copy data into temporary buffer with replacing
              * "version" "value"; to "version" "110";
              */
             if (search_ptr)
               {
                  /*
                   * Calculating new size:
                   * size before "version" + size of new string + size of rest code
                   */
                  new_code = mem_calloc(1, (version_ptr - code) + data_len + strlen(code + concat_pos));

                  /* Copying code, that was before "version" */
                  memcpy(new_code, code, (version_ptr - code));

                  /* Add string: "version" "110"; */
                  memcpy(new_code + (version_ptr - code), data_str, data_len);

                  /* Add rest of source code */
                  memcpy(new_code + data_len + (version_ptr - code), code + concat_pos,
                         strlen(code + concat_pos));

                  fputs(new_code, f);
                  free(new_code);
               }
          }
        free(code);
        fclose(f);

        wm_widgets_list_objects_del(ap->project->widgets);
        thread = pm_project_enventor_save(ap->project,
                                          _progress_print,
                                          _progress_end,
                                          data);
        pm_project_changed(ap->project);
     }
   else
     {
#endif /* HAVE_ENVENTOR */
         thread = pm_project_save(ap->project,
                                  _progress_print,
                                  _progress_end,
                                  data);
#ifdef HAVE_ENVENTOR
     }
#endif /* HAVE_ENVENTOR */
   if (!thread) return false;

   return true;
}

static Eina_Bool
_teardown_save_splash(void *data, Splash_Status status)
{
   App_Data *ap = (App_Data *) data;

   if (status == SPLASH_SUCCESS)
     STATUSBAR_PROJECT_SAVE_TIME_UPDATE(ap);

   ap->project->changed = false;
   workspace_edit_object_recalc(ap->workspace);

   ecore_main_loop_quit();
   return true;
}

void
project_save(void)
{
   App_Data *ap;

   ap = app_data_get();
   if (!ap->project) return;
#ifdef HAVE_ENVENTOR
   if (!ap->enventor_mode)
#endif /* HAVE_ENVENTOR */
     if (!ap->project->changed) return;
   if (ap->splash) return;

   ap->splash = splash_add(ap->win, _setup_save_splash, _teardown_save_splash, NULL, ap);
   evas_object_focus_set(ap->splash, true);
   evas_object_show(ap->splash);

#ifdef HAVE_ENVENTOR
   if (!ap->enventor_mode)
#endif /* HAVE_ENVENTOR */
     ui_menu_disable_set(ap->menu, MENU_FILE_SAVE, true);
     ecore_main_loop_begin();
}

/******************************************************************************/

void
project_changed(void)
{
   App_Data *ap;

   ap = app_data_get();

   if (ap->project->changed) return;
   pm_project_changed(ap->project);
   ui_menu_disable_set(ap->menu, MENU_FILE_SAVE, false);
}

/******************************************************************************/

/*TODO: I think, this functionality need move to dialogs */
static void
_replace_cb(void *data,
            Evas_Object *obj __UNUSED__,
            void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   *res = true;
   ecore_main_loop_quit();
}

static void
_ecancel_cb(void *data,
            Evas_Object *obj __UNUSED__,
            void *ei __UNUSED__)
{
   Eina_Bool *res = data;
   *res = false;
   ecore_main_loop_quit();
}


Eina_Bool
export_replace_request(Evas_Object *parent, const char *msg)
{
   Eina_Bool result = false;
   Evas_Object *popup, *btn, *label;
   Eina_Stringshare *title;

   title = eina_stringshare_printf(_("Export project"));
   popup = elm_popup_add(parent);
   elm_object_style_set(popup, "eflete");
   elm_object_part_text_set(popup, "title,text", title);
   LABEL_ADD(popup, label, msg);
   elm_object_content_set(popup, label);
   BUTTON_ADD(popup, btn, _("Replace"));
   evas_object_smart_callback_add(btn, "clicked", _replace_cb, &result);
   elm_object_part_content_set(popup, "button1", btn);
   BUTTON_ADD(popup, btn, _("Cancel"));
   evas_object_smart_callback_add(btn, "clicked", _ecancel_cb, &result);
   elm_object_part_content_set(popup, "button2", btn);
   evas_object_show(popup);
   eina_stringshare_del(title);

   ecore_main_loop_begin();

   evas_object_del(popup);

   return result;
}


static Eina_Bool
_export_splash_setup(void *data, Splash_Status status __UNUSED__)
{
   App_Data *ap;
   Project_Thread *thread;
   const char *path;

   ap = app_data_get();
   path = (const char *)data;
   thread = pm_project_develop_export(ap->project, path,
                                      _progress_print, _progress_end,
                                      ap);
   if (!thread) return false;

   return true;
}

static Eina_Bool
_export_splash_teardown(void *data, Splash_Status status __UNUSED__)
{
   eina_stringshare_del(data);
   return true;
}

static void
_on_export_done(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   const char *selected;
   Eet_File *ef;
   Evas_Object *win;
   App_Data *ap;

   ap = app_data_get();
   win = (Evas_Object *)data;
   selected = (const char *)event_info;
   if (!selected) goto close;
   fprintf(stdout, "selected file: '%s'\n", selected);
   /* check the existing file */
   ef = eet_open(selected, EET_FILE_MODE_READ);
   if (ef)
     {
        eet_close(ef);
        if (!export_replace_request(win, _("The file already exists.  Replacing it will overwrite its contents.")))
          return;
     }
   ap->splash = splash_add(ap->win, _export_splash_setup, _export_splash_teardown, NULL, (void *)eina_stringshare_add(selected));
   evas_object_focus_set(ap->splash, true);
   evas_object_show(ap->splash);

close:
   evas_object_del(win);
}

static Eina_Bool
_edje_filter(const char *path,
             Eina_Bool dir,
             void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".edj"))
     return true;
   return false;
}

void
project_export_develop(void)
{
   Evas_Object *win, *bg, *fs;
   App_Data *ap = app_data_get();

   ap->modal_editor++;
   MODAL_WINDOW_ADD(win, main_window_get(), _("Export edj file (develop)"), _fs_close, ap);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);
   FILESELECTOR_ADD(fs, win, _on_export_done, win);
   elm_fileselector_is_save_set(fs, true);
   elm_fileselector_custom_filter_append(fs, _edje_filter, NULL, "Edje Files");
   elm_fileselector_mime_types_filter_append(fs, "*", "All Files");
   elm_win_resize_object_add(win, fs);
}

static void
_on_export_edc_group_done(void *data,
                          Evas_Object *obj,
                          void *event_info)
{
   const char *selected;
   Evas_Object *win;
   Eina_Stringshare *path = NULL;
   Eina_Stringshare *dir_path = NULL;
   App_Data *ap;
   Style *style = NULL;

   char **tmp;
   unsigned int tokens_count, i;

   ap = app_data_get();
   win = (Evas_Object *)data;
   selected = (const char *)event_info;
   if (!selected)
     {
        evas_object_del(win);
        return;
     }
   style = ap->project->current_style;

   Eina_Stringshare *file = NULL;

   tmp = eina_str_split_full(style->full_group_name, "/", 0, &tokens_count);
   if (!tmp[0]) return;
   file = eina_stringshare_add(tmp[0]);

   for (i = 1; tokens_count - 1 > 0; i++, tokens_count--)
     file = eina_stringshare_printf("%s_%s", file, tmp[i]);
   free(tmp[0]);
   free(tmp);

   dir_path = eina_stringshare_printf("%s/%s", elm_fileselector_path_get(obj), file);
   if (!ecore_file_exists(dir_path))
     ecore_file_mkdir(dir_path);
   path = eina_stringshare_printf("%s/%s.edc", dir_path, file);
   pm_project_style_source_code_export(ap->project, style, path);
   pm_style_resource_export(ap->project, style, dir_path);

   eina_stringshare_del(file);
   eina_stringshare_del(path);
   eina_stringshare_del(dir_path);
   evas_object_del(win);
}

void
project_export_edc_group(void)
{
   Evas_Object *win, *bg, *fs;
   App_Data *ap = app_data_get();

   ap->modal_editor++;
   MODAL_WINDOW_ADD(win, main_window_get(), _("Export edc file (group)"), _fs_close, ap);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);
   FILESELECTOR_ADD(fs, win, _on_export_edc_group_done, win);
   elm_fileselector_is_save_set(fs, false);
   elm_fileselector_folder_only_set(fs, true);
   elm_win_resize_object_add(win, fs);
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
   App_Data *ap = app_data_get();
   evas_object_hide(ap->popup);
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
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
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

   ap->project->close_request = false;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
   evas_object_del(ap->popup);
   ap->popup = NULL;

   return result;
}

/******************************************************************************/

static Eina_Bool
_selected_layout_delete(Evas_Object *genlist, App_Data *ap)
{
   Elm_Object_Item *item_to_del = elm_genlist_selected_item_get(genlist), *eoi;
   Style *style = elm_object_item_data_get(item_to_del);;
   Style *style_work = NULL;
   Eina_Inlist *l = NULL;
   int i;

   if (!item_to_del)
     {
        NOTIFY_INFO(3, _("Select item to be deleted"));
        return false;
     }

   eoi = elm_genlist_item_next_get(item_to_del);
   if (!eoi)
     {
        eoi = elm_genlist_item_prev_get(item_to_del);
        i = elm_genlist_item_index_get(eoi);
     }
   else
     {
        i = elm_genlist_item_index_get(eoi);
        i--;
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
   genlist = _widgetlist_current_genlist_get(ap, LAYOUT);

   for (eoi = elm_genlist_first_item_get(genlist);
        eoi != elm_genlist_item_next_get(elm_genlist_last_item_get(genlist));
        eoi = elm_genlist_item_next_get(eoi))
     {
        if (elm_genlist_item_index_get(eoi) == i)
          {
             style = elm_object_item_data_get(eoi);;
             elm_genlist_item_selected_set(eoi, true);
             style = elm_object_item_data_get(eoi);;
             break;
          }
     }

   project_changed();
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

   /* Check which type of node is selected: style or class */
   if (elm_genlist_item_parent_get(eoi))
     {
        style = elm_object_item_data_get(eoi);
        class_st = style->parent;
     }
   else
     class_st = elm_object_item_data_get(eoi);

   if (class_st->__type != CLASS) return false;

   /* try to find layout. */
   EINA_INLIST_FOREACH(ap->project->layouts, style_work)
     {
        if (!style_work->isAlias)
          goto found;
     }
   /* Search edje edit object, which wont be deleted now. This object is needed
      for manipulation with group in *.edj file*/
   EINA_INLIST_FOREACH(ap->project->widgets, widget_work)
     {
        EINA_INLIST_FOREACH(widget_work->classes, class_work)
          {
             if (style == NULL)
               {
                  if (class_work->name != class_st->name)
                    {
                       EINA_INLIST_FOREACH(class_work->styles, style_work)
                         goto found;
                    }
               }
             else
               EINA_INLIST_FOREACH(class_work->styles, style_work)
                {
                   if (style_work->full_group_name == style->full_group_name)
                     continue;
                   else
                     {
                        if (!style_work->isAlias)
                          goto found;
                     }
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
   project_changed();
   ui_widget_list_class_data_reload(genlist, widget->classes);

   /* deleting widget */
   if (elm_genlist_items_count(genlist) == 0)
     {
        Evas_Object *widget_list = ui_block_widget_list_get(ap);
        Evas_Object *naviframe = evas_object_data_get(widget_list, "nf_widgets");
        Elm_Object_Item *item_gl_widgets = elm_naviframe_item_pop(naviframe);
        elm_naviframe_item_pop_to(item_gl_widgets);
        genlist = elm_object_item_part_content_get(elm_naviframe_top_item_get(naviframe),
                                                   "elm.swallow.content");
        eoi = elm_genlist_selected_item_get(genlist);
        elm_object_item_del(eoi);
     }

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
   workspace_zoom_factor_set(ap->workspace, 1.0);
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
   workspace_zoom_factor_set(ap->workspace, 1.0);
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
_panes_pos_setup(Evas_Object *panes, double value, Eina_Bool fixed)
{
   if (!panes) return;

   elm_panes_content_left_size_set(panes, value);
   elm_panes_fixed_set(panes, fixed);
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

