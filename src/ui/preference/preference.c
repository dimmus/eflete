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
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_locked_set(ap->menu_hash, false);
}

static void
_on_preferences_close(void *data,
                      Evas *e __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Preferences *preference = (Preferences *)data;
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
#define prof_general pr->general

/* TODO: add field name into Profile struct  */

static void
_general_form(Preferences *preference)
{
   Evas_Object *panes, *layout, *profile;
   Evas_Object *entry_home, *entry_swap;
   Profile *pr;
   /*
    * Eina_List *prof_list, *l;
    * const char *version;
    * prof_list = profiles_get();
    */
   pr = profile_get();
   panes = evas_object_data_get(preference->mwin, "panes");
   layout = elm_object_part_content_get(panes, "right");
   if (layout)
     evas_object_hide(layout);
   if (!pref_layout.general)
     {
        pref_layout.general = elm_layout_add(preference->mwin);
        evas_object_size_hint_weight_set(pref_layout.general, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_layout_file_set(pref_layout.general, EFLETE_EDJ, "elm/layout/preferences/general");
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
   EWE_ENTRY_ADD(pref_layout.general, entry_home, true, DEFAULT_STYLE);
   elm_object_part_text_set(entry_home, "elm.text", prof_general.home_folder);
   elm_object_part_content_set(pref_layout.general, "swallow.home", entry_home);
   EWE_ENTRY_ADD(pref_layout.general, entry_swap, true, DEFAULT_STYLE);
   elm_object_part_text_set(entry_swap, "elm.text", prof_general.swap_folder);
   elm_object_part_content_set(pref_layout.general, "swallow.swap", entry_swap);
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
        _itc->item_style = "eflete/level1";
        _itc->func.text_get = _item_label_get;
        _itc->func.content_get = NULL;
        _itc->func.state_get = NULL;
        _itc->func.del = NULL;
     }

   BOX_ADD(preference->mwin, box, false, false);
   preference->glist = elm_genlist_add(box);

   preference->glist = elm_genlist_add(box);
   elm_object_style_set(preference->glist, DEFAULT_STYLE);
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

   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   preference = (Preferences *)mem_calloc(1, sizeof(Preferences));

   preference->pr = project;
   preference->mwin = mw_add(_on_edit_preferences_exit, preference);
   mw_title_set(preference->mwin, _("Preferences"));
   evas_object_event_callback_add(preference->mwin, EVAS_CALLBACK_FREE,
                                  _on_preferences_close, preference);
   window_layout = elm_layout_add(preference->mwin);
   elm_layout_file_set(window_layout, EFLETE_EDJ, "eflete/editor/default");
   elm_win_inwin_content_set(preference->mwin, window_layout);

   panes = elm_panes_add(preference->mwin);
   elm_object_style_set(panes, DEFAULT_STYLE);
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

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_event_callback_add(preference->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(preference->mwin);
   return preference->mwin;
}
