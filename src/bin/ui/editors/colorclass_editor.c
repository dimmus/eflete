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

#include "colorclass_editor.h"
#include "main_window.h"

TODO("Rename this file to colorclass_manager")

static Elm_Genlist_Item_Class *_itc_ccl = NULL;

typedef struct _Colorclasses_Editor Colorclasses_Editor;
typedef struct _Colorclass_Item Colorclass_Item;
typedef struct _Search_Data Search_Data;

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Colorclasses_Editor
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *edit_obj;
   Evas_Object *edje_preview;
   Eina_Bool changed;
   Search_Data style_search_data;
};

struct _Colorclass_Item
{
   Eina_Stringshare *name;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
};

/* Modal Window callbacks (closing and exiting from colorclass manager) */
static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
   ap->modal_editor--;
}
static void
_on_btn_cancel(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Colorclasses_Editor *edit = (Colorclasses_Editor *)data;
   mw_del(edit->mwin);
}

/* Search functions and creatings */
ITEM_SEARCH_FUNC(genlist,ELM_GENLIST_ITEM_SCROLLTO_MIDDLE, "elm.text")
static inline Evas_Object *
_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;
   ENTRY_ADD(parent, entry, true);
   elm_object_style_set(entry, "search_field");
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}
static void
_search_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Colorclasses_Editor *edit = data;
   _genlist_item_search(edit->genlist, &(edit->style_search_data),
                        edit->style_search_data.last_item_found);
}
static void
_search_nxt_gd_item(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Colorclasses_Editor *edit = data;
   Elm_Object_Item *start_from = NULL;

   if (edit->style_search_data.last_item_found)
     start_from = elm_genlist_item_next_get(edit->style_search_data.last_item_found);

   _genlist_item_search(edit->genlist, &(edit->style_search_data), start_from);
}
static void
_search_reset_cb(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Search_Data *search_data = data;
   search_data->last_item_found = NULL;
}

/* Creating main layout of Manager and filling with data
 (with callbacks for genlist also) */
static char *
_item_ccl_label_get(void *data,
                    Evas_Object *obj __UNUSED__,
                    const char *part __UNUSED__)
{
   Colorclass_Item *ccl_it = (Colorclass_Item *)data;
   return strdup(ccl_it->name);
}
static void
_item_ccl_del(void *data,
                    Evas_Object *obj __UNUSED__)
{
   Colorclass_Item *ccl_it = (Colorclass_Item *)data;
   eina_stringshare_del(ccl_it->name);
   free(ccl_it);
   ccl_it = NULL;
}
static void
_colorclass_main_layout_create(Colorclasses_Editor *edit)
{
   Evas_Object *search, *bg;

   /* Creating main layout of window */
   edit->layout = elm_layout_add(edit->mwin);
   elm_layout_theme_set(edit->layout, "layout", "colorclass_editor", "default");
   evas_object_size_hint_weight_set(edit->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->layout);
   elm_win_inwin_content_set(edit->mwin, edit->layout);

   /* List of project's colorclasses */
   edit->genlist = elm_genlist_add(edit->layout);
   evas_object_size_hint_weight_set(edit->genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(edit->genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(edit->genlist);
   elm_object_part_content_set(edit->layout, "swallow.list", edit->genlist);

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = "default";
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.del = _item_ccl_del;
     }

   /* Search engine */
   search = _editor_search_field_create(edit->layout);
   elm_object_part_content_set(edit->layout, "swallow.search", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, edit);
   evas_object_smart_callback_add(edit->genlist, "pressed", _search_reset_cb,
                                  &(edit->style_search_data));
   edit->style_search_data.search_entry = search;
   edit->style_search_data.last_item_found = NULL;

   /* Entry preview to show colorclass */
   IMAGE_ADD_NEW(edit->layout, bg, "bg", "tile");
   elm_object_part_content_set(edit->layout, "swallow.entry.bg", bg);
   evas_object_show(bg);

   edit->edje_preview = edje_object_add(evas_object_evas_get(edit->mwin));
   if (!edje_object_file_set(edit->edje_preview,
                             EFLETE_THEME,
                             "elm/layout/colorclass_manager/preview"))
     ERR("Couldn't load layout for text example field!");
   edje_object_part_text_set(edit->edje_preview, "text_example",
                       _("The quick brown fox jumps over the lazy dog"));
   evas_object_size_hint_align_set(edit->edje_preview, -1, -1);
   evas_object_show(edit->edje_preview);
   elm_object_part_content_set(edit->layout, "swallow.entry", edit->edje_preview);
}
Eina_Bool
_colorclass_viewer_init(Colorclasses_Editor *edit)
{
   int r1, r2, r3, g1, g2, g3, b1, b2, b3, a1, a2, a3;
   const char *ccname = NULL;
   Eina_List *cclist = NULL;
   Eina_List *l = NULL;
   Colorclass_Item *it = NULL;
   Evas_Object *edje_edit_obj = NULL;

   GET_OBJ(edit->pr, edje_edit_obj);
   edit->edit_obj = edje_edit_obj;
   cclist = edje_edit_color_classes_list_get(edje_edit_obj);

   EINA_LIST_FOREACH(cclist, l, ccname)
     {
        it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
        if (!edje_edit_color_class_colors_get(edje_edit_obj, ccname,
                                     &r1, &g1, &b1, &a1,
                                     &r2, &g2, &b2, &a2,
                                     &r3, &g3, &b3, &a3))
          {
             free(it);
             ERR("Couldn`t get color's from colorclass %s.", ccname);
             continue;
          }
        it->r1 = r1; it->g1 = g1;
        it->b1 = b1; it->a1 = a1;
        it->r2 = r2; it->g2 = g2;
        it->b2 = b2; it->a2 = a2;
        it->r3 = r3; it->g3 = g3;
        it->b3 = b3; it->a3 = a3;

        it->name = eina_stringshare_add(ccname);
        elm_genlist_item_append(edit->genlist, _itc_ccl, it, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   eina_list_free(cclist);
   return true;
}

Evas_Object *
colorclass_viewer_add(Project *project)
{
   Evas_Object *ic;
   Colorclasses_Editor *edit = NULL;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   if (!project)
     {
        ERR("Project isn't opened");
        return NULL;
     }

   edit = (Colorclasses_Editor *)mem_calloc(1, sizeof(Colorclasses_Editor));
   edit->changed = false;
   edit->pr = project;
   edit->mwin = mw_add(_on_btn_cancel, edit);
   if (!edit->mwin)
     {
        free(edit);
        return NULL;
     }
   mw_title_set(edit->mwin, _("Color class manager"));
   ic = elm_icon_add(edit->mwin);
   elm_icon_standard_set(ic, "color");
   mw_icon_set(edit->mwin, ic);

   _colorclass_main_layout_create(edit);
   if (!_colorclass_viewer_init(edit))
     {
        NOTIFY_ERROR(_("Failed initialize colorclasses editor"));
        free(edit);
        return NULL;
     }

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(edit->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(edit->mwin);

   ap->modal_editor++;

   return edit->mwin;
}
