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

#include "ui_main_window.h"

static void
_add_part_dialog(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   part_dialog_add(ap);
}

static void
_del_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Style *style = ap->project->current_style;
   if (!style) return;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   if ((part) && (workspace_edit_object_part_del(ap->workspace, part->name)))
     ui_widget_list_selected_part_del(ui_block_widget_list_get(ap), style);
   live_view_widget_style_set(ap->live_view, ap->project, style);
}

static void
_above_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Style *style = ap->project->current_style;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   if ((part) && (workspace_edit_object_part_above(ap->workspace, part->name)))
      ui_widget_list_selected_part_above(ui_block_widget_list_get(ap), style);
   live_view_widget_style_set(ap->live_view, ap->project, style);
}

static void
_below_part(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Style *style = ap->project->current_style;
   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   if ((part) && (workspace_edit_object_part_below(ap->workspace, part->name)))
      ui_widget_list_selected_part_below(ui_block_widget_list_get(ap), style);
   live_view_widget_style_set(ap->live_view, ap->project, style);
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
_del_layout(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   return;
}

static void
_add_layout_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Evas_Object *widget_list = (Evas_Object *)event_info;
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
   layout = wm_style_add(name, name, LAYOUT);
   layout->isModify = true;
   ap->project->layouts = eina_inlist_append(ap->project->layouts,
                                             EINA_INLIST_GET(layout));

   wm_style_data_load(layout, evas_object_evas_get(widget_list),
                      ap->project->swapfile);
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
   ui_style_delete(ap);
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

void
ui_part_back(App_Data *ap)
{
   if (!ap) return;

   Evas_Object *wl_list, *groupedit;

   wl_list = ui_block_widget_list_get(ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,add", _add_part_dialog, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,del", _del_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,above", _above_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,below", _below_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,show", _show_part, ap);
   evas_object_smart_callback_del_full(wl_list, "wl,part,hide", _hide_part, ap);

   groupedit = ws_groupedit_get(ap->workspace);
   evas_object_smart_callback_add(groupedit, "object,area,changed", _live_view_update, ap);

   workspace_edit_object_unset(ap->workspace);
   ui_states_list_data_unset(ui_block_state_list_get(ap));
   ui_signal_list_data_unset(ui_block_signal_list_get(ap));
   ui_property_style_unset(ui_block_property_get(ap));
   /*TODO: in future it will be moved to block api. */
   elm_object_signal_emit(ap->block.bottom_left, "title,content,hide", "eflete");
   live_view_widget_style_unset(ap->live_view);

   ui_menu_disable_set(ap->menu_hash, _("Programs"), true);

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

   if ((!ap) && (!obj))
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
ui_part_select(App_Data *ap,
               Part* part)
{
   Evas_Object *prop;
   Evas_Object *gl_states;

   if ((!ap) && (!part))
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

   ui_property_state_set(prop, part);
   workspace_edit_object_part_state_set(ap->workspace, part);
   workspace_highlight_unset(ap->workspace);
   workspace_highlight_set(ap->workspace, part);
   evas_object_smart_callback_del_full(ap->workspace, "part,changed", _property_change, ap);
   evas_object_smart_callback_add(ap->workspace, "part,changed", _property_change, ap);

   return gl_states;
}

/* FIXME: rename to style_clicked */
void
ui_style_clicked(App_Data *ap, Style *style)
{
   Evas_Object *wl_list = NULL;
   Evas_Object *gl_signals = NULL;
   Evas_Object *prop = NULL;
   Evas_Object *groupedit = NULL;
   Eina_List *signals = NULL;
   Style *_style = NULL, *_alias_style = NULL;

   if ((!ap) && (!ap->project) && (!style))
     {
        ERR("App Data or style is missing!");
        return;
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

   /* Get signals list of a styles and show them */
   gl_signals = ui_signal_list_add(ap->block.left_bottom);
   ui_signal_list_data_set(gl_signals, _style);
   wm_program_signals_list_free(signals);
   ui_block_signal_list_set(ap, gl_signals);

   workspace_edit_object_set(ap->workspace, _style, ap->project->swapfile);
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
   ui_property_style_set(prop, _alias_style, ap->workspace);
   evas_object_show(prop);

   live_view_widget_style_set(ap->live_view, ap->project, _style);
   ui_menu_disable_set(ap->menu_hash, _("Programs"), false);
}

Evas_Object *
ui_edj_load_done(App_Data* ap, const char *selected)
{
   Evas_Object *wd_list = NULL;
   char *name;

   if ((!ap) || (!selected)) return NULL;

   if (eina_str_has_suffix(selected, ".edj"))
     {
        INFO("Selected file: %s", selected);
        ui_property_style_unset(ui_block_property_get(ap));
        ui_states_list_data_unset(ui_block_state_list_get(ap));
        ui_signal_list_data_unset(ui_block_signal_list_get(ap));

        if (ap->workspace)
          {
             workspace_edit_object_unset(ap->workspace);
             workspace_highlight_unset(ap->workspace);
          }

        if (ap->live_view) live_view_widget_style_unset(ap->live_view);
        GET_NAME_FROM_PATH(name, selected)
        ap->project = pm_open_project_edj(name, selected);
        free(name);
        if (!ap->project)
          {
             NOTIFY_ERROR(_("Can't open file: %s"), selected);
             return NULL;
          }
        NOTIFY_INFO(3, _("Selected file: %s"), selected);

        wd_list = ui_widget_list_add(ap->win);
        ui_widget_list_title_set(wd_list, ap->project->name);
        ui_widget_list_data_set(wd_list, ap->project);
        ui_block_widget_list_set(ap, wd_list);
        evas_object_show(wd_list);
        ui_panes_show(ap);

        ui_menu_base_disabled_set(ap->menu_hash, false);
     }
   else NOTIFY_ERROR(_("The file must have a extension '.edj'"));

   return wd_list;
}

Eina_Bool
new_theme_create(App_Data *ap)
{
   Evas_Object *wd_list = NULL;

   if (!ap) return false;

   ap->is_new = false;

   if (!ecore_file_cp(EFLETE_EDJ_PATH"template.edj", EFLETE_SWAP_PATH"Untitled.edj"))
     {
        ERR("Couldn't copy theme template to cache");
        return false;
     }

   Evas_Object *prop, *state, *signal;
   state = ui_block_state_list_get(ap);
   if (state) elm_genlist_clear(state);
   signal = ui_block_signal_list_get(ap);
   if (signal) elm_genlist_clear(signal);
   prop = ui_block_property_get(ap);
   if (prop) ui_property_style_unset(prop);

   if (ap->workspace)
     {
        workspace_edit_object_unset(ap->workspace);
        workspace_highlight_unset(ap->workspace);
     }

   if ((ap->live_view) || (ap->project))
     live_view_widget_style_unset(ap->live_view);
   ui_menu_disable_set(ap->menu_hash, _("Programs"), true);

   ap->project = pm_open_project_edj("Untitled.edj", EFLETE_SWAP_PATH"Untitled.edj");
   if (!ap->project) return false;
   wd_list = ui_widget_list_add(ap->win);
   ui_widget_list_title_set(wd_list, ap->project->name);
   ui_widget_list_data_set(wd_list, ap->project);
   ui_block_widget_list_set(ap, wd_list);
   add_callbacks_wd(wd_list, ap);
   evas_object_show(wd_list);
   ui_panes_show(ap);
   ap->project->edj = NULL;

   ui_menu_base_disabled_set(ap->menu_hash, false);

   return true;
}

Eina_Bool
ui_style_delete(App_Data *ap)
{
   Widget *widget = NULL;
   Class *class_st = NULL;
   Class *class_work = NULL;
   Style *style = NULL;
   Style *style_work = NULL;

   Evas_Object *gl_style = NULL;
   Evas_Object *gl_widget = NULL;
   Evas_Object *nf = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *eoi_work = NULL;
   Evas_Object *box = NULL;
   Eina_List *box_childs = NULL;
   Eina_Inlist *l = NULL;
   int inlist_count = 0;

   if (!ap) return false;

   nf = ui_block_widget_list_get(ap);
   eoi = elm_naviframe_bottom_item_get(nf);
   gl_widget = elm_object_item_part_content_get(eoi, NULL);
   eoi = elm_genlist_selected_item_get(gl_widget);
   widget = elm_object_item_data_get(eoi);

   eoi = elm_naviframe_top_item_get(nf);
   box = elm_object_item_part_content_get(eoi, NULL);
   box_childs = elm_box_children_get(box);
   gl_style = eina_list_data_get(eina_list_last(box_childs));
   eoi = elm_genlist_selected_item_get(gl_style);

   if (!eoi)
     {
        NOTIFY_INFO(3, _("No one style is selected"));
        return false;
     }
   eoi_work = elm_genlist_item_parent_get(eoi);
   if (eoi_work)
      class_st = elm_object_item_data_get(eoi_work);
   else
      class_st = elm_object_item_data_get(eoi);

   if (!strcmp(class_st->name, "base"))
     {
        NOTIFY_INFO(3, _("Couldn't delete anything from base class"));
        return false;
     }

   if (eoi_work)
     {
        inlist_count = eina_inlist_count(class_st->styles);
        if (inlist_count <= 1)
          {
             NOTIFY_INFO(3, _("Couldn't delete last class in style. Try to delete style"));
             return false;
          }

        style = elm_object_item_data_get(eoi);
        if (!edje_edit_group_exist(style->obj, style->full_group_name))
          {
             NOTIFY_INFO(3, _("Style [%s] don't exist"), style->name);
             return false;
          }

        EINA_INLIST_FOREACH_SAFE(class_st->styles, l, style_work)
          {
             if (strcmp(style->full_group_name, style_work->full_group_name))
               break;
          }

        if (!style_work) return false;

        evas_object_del(style->obj);
        if (!edje_edit_group_del(style_work->obj, style->full_group_name))
          {
             NOTIFY_INFO(3, _("Failed to delete class[%s]"), style->name);
             return false;
          }
        class_st->styles = eina_inlist_remove(class_st->styles, EINA_INLIST_GET(style));
        wm_style_free(style);
     }
   else
     {
        inlist_count = eina_inlist_count(widget->classes);
        if (inlist_count <= 1)
          {
             NOTIFY_INFO(3, _("Couldn't delete last class in widget."));
             return false;
          }

        EINA_INLIST_FOREACH_SAFE(widget->classes, l, class_work)
          {
             if (!class_work) continue;
             if (strcmp(class_st->name, class_work->name))
               break;
          }

        if (!class_work) return false;

        style_work = EINA_INLIST_CONTAINER_GET(class_work->styles, Style);
        if (!style_work) return false;

        EINA_INLIST_FOREACH_SAFE(class_st->styles, l, style)
          {
             if (!style)
               {
                  ERR("NULL style pointer.");
                  continue;
               }

             evas_object_del(style->obj);
             if (!edje_edit_group_del(style_work->obj, style->full_group_name))
               {
                  NOTIFY_INFO(3, _("Failed to delete style[%s] in class [%s]"),
                              style->name, class_st->name);
               }
          }
        widget->classes = eina_inlist_remove(widget->classes, EINA_INLIST_GET(class_st));
        wm_class_free(class_st);
     }
   ui_widget_list_class_data_reload(gl_style, widget->classes);
   return true;
}

Eina_Bool
ui_part_state_delete(App_Data *ap)
{
   Evas_Object *state_list = NULL;
   Part *part = NULL;
   Style *style = NULL;
   Elm_Object_Item *eoi = NULL;
   char **arr = NULL;
   Eina_Stringshare *full_state_name = NULL;
   Eina_Stringshare *state_name = NULL;
   Eina_Stringshare *state_value = NULL;
   double value = 0;

#define CLEAR_STRINGS \
        eina_stringshare_del(state_name); \
        eina_stringshare_del(state_value); \
        eina_stringshare_del(full_state_name); \
        free(arr[0]); \
        free(arr);

   if ((!ap) && (!ap->workspace)) return false;

   state_list = ui_block_state_list_get(ap);
   part = ui_states_list_part_get(state_list);
   if (!part) return false;

   eoi = elm_genlist_selected_item_get(state_list);
   if (!eoi)
     {
        NOTIFY_INFO(3, _("Please select part state"));
        return false;
     }

   full_state_name = eina_stringshare_add(elm_object_item_data_get(eoi));
   if (!full_state_name) return false;

   arr = eina_str_split(full_state_name, " ", 3);
   state_name = eina_stringshare_add(arr[0]);
   state_value = eina_stringshare_add(arr[1]);
   value = atof(state_value);

   if (!edje_edit_state_del(style->obj, part->name, state_name, value))
     {
        if ((!strcmp(state_name, "default")) && (value == 0))
          {
             NOTIFY_WARNING(_("Couldn't delete default state"));
          }
        else
          NOTIFY_WARNING(_("Failed delete state \n[%s %3.2f]"), state_name, value);
        CLEAR_STRINGS;
        return false;
     }
  elm_object_item_del(eoi);
  elm_genlist_item_selected_set(elm_genlist_first_item_get(state_list), true);
  CLEAR_STRINGS;
#undef CLEAR_STRINGS
  return true;
}

Eina_Bool
register_callbacks(App_Data *ap)
{
   if (!ap) return false;

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
