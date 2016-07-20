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

#include "preference.h"
#include "main_window.h"

typedef struct _Preferences Preferences;
static Elm_Genlist_Item_Class *_itc = NULL;

struct _Preferences
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *glist;
   struct
   {
      Evas_Object *general;
      Evas_Object *workspace;
   } layout;
};

static void
_on_edit_preferences_exit(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Preferences *preference = (Preferences *)data;
   evas_object_del(preference->mwin);
}

static void
_on_mwin_del(void * data __UNUSED__,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_on_preferences_close(void *data,
                      Evas *e __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Preferences *preference = (Preferences *)data;
   Config *cf = config_get();

   preferences_project_autosave_update(preference->pr);

   profile_save(cf->profile);
   free(preference);
}

/* For GenList, getting the content for showing. Tag Names. */
static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   char *label = (char *)data;
   if (!label)
     {
        ERR("It impossible, but it is occurred, tag's name is missing!");
        return NULL;
     }
   return strdup(label);
}

#define pref_layout preference->layout
#define prof_general prof->general

#define COMBOBOX_VALUE \
Ewe_Combobox_Item *item = ei; \
value = eina_stringshare_add(item->title);

#define SPINNER_VALUE \
value = elm_spinner_value_get(obj);

#define CHECK_VALUE \
Evas_Object *check = obj; \
value = elm_check_state_get(check);

#define CHANGE_CALLBACK(VALUE, NUM, WIDGET, TYPE) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj __UNUSED__, \
                     void *ei __UNUSED__) \
{ \
   Profile *prof = (Profile *)data; \
   TYPE value; \
   WIDGET##_VALUE \
   switch (NUM) \
     { \
      case (1): \
        prof_general.autosave.period = value; \
        break; \
      case (2): \
        prof_general.autosave.autosave = value; \
        break; \
      case (3): \
        prof_general.save_ui = value; \
        break; \
      case (4): \
        prof_general.save_win_pos = value; \
        break; \
     } \
}

#define CHANGE_ENTRY_CALLBACK(VALUE, NUM) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj, \
                     void *ei __UNUSED__) \
{ \
   Profile *prof = (Profile *)data; \
   const char *value; \
   value = elm_entry_entry_get(obj); \
   switch (NUM) \
     { \
      case (5): \
        prof_general.projects_folder = strdup(value); \
        break; \
     } \
}

CHANGE_CALLBACK(autosave_period, 1, SPINNER, double)
CHANGE_CALLBACK(autosave_autosave, 2, CHECK, Eina_Bool)
CHANGE_CALLBACK(save_ui, 3, CHECK, Eina_Bool)
CHANGE_CALLBACK(save_win_pos, 4, CHECK, Eina_Bool)
CHANGE_ENTRY_CALLBACK(home, 5)

#undef COMBOBOX_VALUE
#undef SPINNER_VALUE
#undef CHECK_VALUE
#undef CHANGE_ENTRY_CALLBACK
#undef CHANGE_CALLBACK

TODO("add field name into Profile struct")

static void
_general_form(Preferences *preference)
{
   Evas_Object *panes, *layout, *profile;
   Evas_Object *entry_home, *autosave_frame, *store_frame;
   Evas_Object *autosave_layout, *store_layout;
   Evas_Object *autosave_ck, *period_sp, *storeui_ck, *winpos_ck;
   Profile *prof;
   /*
    * Eina_List *prof_list, *l;
    * const char *version;
    * prof_list = profiles_get();
    */
   prof = profile_get();
   panes = evas_object_data_get(preference->mwin, "panes");
   layout = elm_object_part_content_get(panes, "right");
   if (layout)
     evas_object_hide(layout);
   if (!pref_layout.general)
     {
        pref_layout.general = elm_layout_add(preference->mwin);
        evas_object_size_hint_weight_set(pref_layout.general, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_layout_theme_set(pref_layout.general, "layout", "preferences", "general");
        elm_object_part_content_set(panes, "right", pref_layout.general);
     }
   evas_object_show(pref_layout.general);

   EWE_COMBOBOX_ADD(pref_layout.general, profile);
   elm_object_part_content_set(pref_layout.general, "swallow.profile", profile);
   ewe_combobox_text_set(profile, _("Loaded profile"));
   elm_object_disabled_set(profile, true);
   /*
    * evas_object_smart_callback_add(profile, "selected", _on_##VALUE##_change, style_edit);
    * EINA_LIST_FOREACH(prof_list, l, pr)
    *    {
    *       version = eina_stringshare_printf("%d", pr->version);
    *       ewe_combobox_item_add(profile, version);
    *    }
    */
   ENTRY_ADD(pref_layout.general, entry_home, true);
   elm_object_part_text_set(entry_home, "elm.text", prof_general.projects_folder);
   elm_object_part_content_set(pref_layout.general, "swallow.home", entry_home);
   evas_object_smart_callback_add(entry_home, "changed", _on_home_change, prof);

   FRAME_ADD(pref_layout.general, autosave_frame, false, _("Autosave"))
   elm_object_part_content_set(pref_layout.general, "swallow.autosave", autosave_frame);
   autosave_layout = elm_layout_add(autosave_frame);
   evas_object_size_hint_weight_set(autosave_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(autosave_layout, "layout", "preferences", "frame");
   evas_object_show(autosave_layout);
   elm_object_content_set(autosave_frame, autosave_layout);
   elm_object_part_text_set(autosave_layout, "label.first", _("autosave"));
   elm_object_part_text_set(autosave_layout, "label.second", _("period"));

   CHECK_ADD(autosave_layout, autosave_ck);
   elm_check_state_set(autosave_ck, prof_general.autosave.autosave);
   elm_object_part_content_set(autosave_layout, "swallow.first", autosave_ck);
   evas_object_smart_callback_add(autosave_ck, "changed", _on_autosave_autosave_change, prof);

   SPINNER_ADD(autosave_layout, period_sp, 1.0, 9999.0, 1.0, true);
   elm_spinner_value_set(period_sp, prof_general.autosave.period);
   elm_object_part_content_set(autosave_layout, "swallow.second", period_sp);
   evas_object_smart_callback_add(period_sp, "changed", _on_autosave_period_change, prof);

   FRAME_ADD(pref_layout.general, store_frame, false, _("UI"))
   elm_object_part_content_set(pref_layout.general, "swallow.uistore", store_frame);
   store_layout = elm_layout_add(store_frame);
   evas_object_size_hint_weight_set(store_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(store_layout, "layout", "preferences", "frame");
   evas_object_show(store_layout);
   elm_object_content_set(store_frame, store_layout);
   elm_object_part_text_set(store_layout, "label.first", _("UI store on exit"));
   elm_object_part_text_set(store_layout, "label.second", _("store window position"));

   CHECK_ADD(store_layout, storeui_ck);
   elm_check_state_set(storeui_ck, prof_general.save_ui);
   elm_object_part_content_set(store_layout, "swallow.first", storeui_ck);
   evas_object_smart_callback_add(storeui_ck, "changed", _on_save_ui_change, prof);

   CHECK_ADD(store_layout, winpos_ck);
   elm_check_state_set(winpos_ck, prof_general.save_win_pos);
   elm_object_part_content_set(store_layout, "swallow.second", winpos_ck);
   evas_object_smart_callback_add(winpos_ck, "changed", _on_save_win_pos_change, prof);
}

static void
_workspace_form(Preferences *preference)
{
   Evas_Object *panes, *layout;
   panes = evas_object_data_get(preference->mwin, "panes");
   layout = elm_object_part_content_get(panes, "right");
   if (layout)
     evas_object_hide(layout);
}

#undef pref_layout
#undef prof_general

static void
_on_glit_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   int count = 0;
   Preferences *preference = (Preferences *)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   count = elm_genlist_item_index_get(glit);
   switch (count)
     {
      case 1:
        {
           _general_form(preference);
           break;
        }
      case 2:
        {
           _workspace_form(preference);
           break;
        }
     }

}

static Evas_Object *
_form_left_side(Preferences *preference)
{
   Evas_Object *box;

   if (!_itc)
     {
        _itc = elm_genlist_item_class_new();
        _itc->item_style = "title";
        _itc->func.text_get = _item_label_get;
        _itc->func.content_get = NULL;
        _itc->func.state_get = NULL;
        _itc->func.del = NULL;
     }

   BOX_ADD(preference->mwin, box, false, false);
   preference->glist = elm_genlist_add(box);

   preference->glist = elm_genlist_add(box);
   elm_box_pack_end(box, preference->glist);
   evas_object_show(preference->glist);

   evas_object_size_hint_align_set(preference->glist, EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(preference->glist, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   elm_genlist_item_append(preference->glist, _itc,
                           _("General"), NULL, ELM_GENLIST_ITEM_NONE,
                           _on_glit_selected, preference);
   elm_genlist_item_append(preference->glist, _itc,
                           _("Workspace"), NULL, ELM_GENLIST_ITEM_NONE,
                           _on_glit_selected, preference);
   elm_genlist_item_append(preference->glist, _itc,
                           _("Live view"), NULL, ELM_GENLIST_ITEM_NONE,
                           _on_glit_selected, preference);

   return box;
}

Evas_Object *
preferences_window_add(Project *project)
{
   Evas_Object *panes;
   Evas_Object *layout_left;
   Evas_Object *window_layout, *button_box, *btn;
   Preferences *preference = NULL;

   preference = (Preferences *)mem_calloc(1, sizeof(Preferences));

   preference->pr = project;
   preference->mwin = mw_add(NULL, _on_edit_preferences_exit, preference);
   if (!preference->mwin)
     {
        free(preference);
        return NULL;
     }
   mw_title_set(preference->mwin, _("Preferences"));
   evas_object_event_callback_add(preference->mwin, EVAS_CALLBACK_FREE,
                                  _on_preferences_close, preference);
   window_layout = elm_layout_add(preference->mwin);
   elm_layout_file_set(window_layout, ap.path.layout_edj, "eflete/editor/default");
   elm_win_inwin_content_set(preference->mwin, window_layout);

   panes = elm_panes_add(preference->mwin);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.2);
   elm_object_part_content_set(window_layout, "eflete.swallow.content", panes);
   evas_object_data_set(preference->mwin, "panes", panes);
   evas_object_show(panes);

   layout_left = _form_left_side(preference);
   elm_object_part_content_set(panes, "left", layout_left);
   evas_object_show(layout_left);

   elm_genlist_item_selected_set(elm_genlist_first_item_get(preference->glist), true);

   BOX_ADD(window_layout, button_box, true, false)
   elm_box_align_set(button_box, 1.0, 0.5);

   BUTTON_ADD(preference->mwin, btn, _("Close"));
   evas_object_smart_callback_add(btn, "clicked", _on_edit_preferences_exit, preference);
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_size_hint_min_set(btn, 100, 30);
   evas_object_show(btn);
   elm_box_pack_end(button_box, btn);
   elm_object_part_content_set(window_layout, "eflete.swallow.button_box", button_box);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(preference->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, NULL);

   evas_object_show(preference->mwin);
   return preference->mwin;
}
