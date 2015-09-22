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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "colorclass_manager.h"
#include "main_window.h"
#include "editor.h"

static Elm_Genlist_Item_Class *_itc_ccl = NULL;

typedef struct _Colorclasses_Manager Colorclasses_Manager;
typedef struct _Colorclass_Item Colorclass_Item;
typedef struct _Search_Data Search_Data;

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Colorclass_Item
{
   Eina_Stringshare *name;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
};

struct _Colorclasses_Manager
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *edje_preview, *preview_layout;
   Evas_Object *colorsel1, *colorsel2, *colorsel3;
   Evas_Object *entry, *popup;
   Evas_Object *btn_add;
   Elm_Validator_Regexp *name_validator;
   Eina_Bool changed;
   Search_Data style_search_data;
   Colorclass_Item *current_ccl;
   Eina_List *unapplied_list;
};

/* BUTTON ADD AND REMOVE FUNCTIONS */
static void
_on_add_popup_btn_add(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);

   Colorclass_Item *it = NULL, *cc_it = NULL;
   Elm_Object_Item *glit_ccl = NULL;
   Uns_List *colorclass = NULL;
   Eina_List *l;
   Colorclass_Resource *res;

   it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   it->name = elm_entry_entry_get(edit->entry);

   Elm_Object_Item *start_from = elm_genlist_first_item_get(edit->genlist);
   if (elm_genlist_search_by_text_item_get(edit->genlist, start_from, "elm.text",
                                           it->name, 0))
     {
        NOTIFY_WARNING(_("Color class name must be unique!"));
        free(it);
        return;
     }

   res = mem_calloc(1, sizeof(Colorclass_Resource));
   res->name = eina_stringshare_add(it->name);
   ap.project->colorclasses = eina_list_sorted_insert(ap.project->colorclasses,
                                                      (Eina_Compare_Cb) resource_cmp,
                                                      res);

TODO("REMOVE THIS SINCE IT'S NO MORE!")
   EINA_LIST_FOREACH(edit->unapplied_list, l, colorclass)
     {
        cc_it = (Colorclass_Item *)colorclass->data;

        if ((colorclass->act_type == ACTION_TYPE_DEL) &&
            (!strcmp(it->name, cc_it->name)))
          {
             edit->unapplied_list = eina_list_remove_list(edit->unapplied_list, l);
             eina_stringshare_del(cc_it->name);
             free(cc_it);
             free(colorclass);
             cc_it = NULL;
             colorclass = NULL;
             break;
          }
        cc_it = NULL;
        colorclass = NULL;
     }

   /* just a copy for list of unapplied, because "it" would be deleted. */
   cc_it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   cc_it->name = eina_stringshare_add(it->name);

TODO("Remove this as well")
   colorclass = mem_malloc(sizeof(Uns_List));
   colorclass->data = cc_it;
   colorclass->act_type = ACTION_TYPE_ADD;
   edit->unapplied_list = eina_list_append(edit->unapplied_list, colorclass);

   edje_edit_color_class_add(ap.project->global_object, eina_stringshare_add(cc_it->name));

   glit_ccl = elm_genlist_item_append(edit->genlist, _itc_ccl, it, NULL,
                                    ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_ccl, EINA_TRUE);

   evas_object_del(edit->popup);
   edit->popup = NULL;
TODO("Surely delete this one below")
   edit->changed = true;

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
}
static void
_on_add_popup_btn_cancel(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);

   evas_object_del(edit->popup);
   elm_validator_regexp_free(edit->name_validator);
   edit->name_validator = NULL;

   edit->popup = NULL;
}

static void
_validation(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);

   if (elm_validator_regexp_status_get(edit->name_validator) != ELM_REG_NOERROR)
     elm_object_disabled_set(edit->btn_add, true);
   else
     elm_object_disabled_set(edit->btn_add, false);
}

static void
_on_button_add_clicked_cb(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Evas_Object *box, *bt_no, *item;
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);
   assert(edit->name_validator == NULL);

   edit->popup = elm_popup_add(ap.win);
   elm_object_part_text_set(edit->popup, "title,text", _("Add color class:"));

   edit->name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   box = elm_box_add(edit->popup);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

   LAYOUT_PROP_ADD(edit->popup, _("Color class name: "), "property", "1swallow")
   ENTRY_ADD(item, edit->entry, true);
   eo_do(edit->entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, edit->name_validator));
   evas_object_smart_callback_add(edit->entry, "changed", _validation, edit);
   elm_object_part_text_set(edit->entry, "guide", _("Type new color class name here"));
   elm_object_part_content_set(item, "elm.swallow.content", edit->entry);

   elm_box_pack_end(box, item);
   elm_object_content_set(edit->popup, box);
   evas_object_show(box);

   BUTTON_ADD(edit->popup, edit->btn_add, _("Ok"));
   evas_object_smart_callback_add(edit->btn_add, "clicked", _on_add_popup_btn_add, edit);
   elm_object_part_content_set(edit->popup, "button1", edit->btn_add);

   BUTTON_ADD(edit->popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add(bt_no, "clicked", _on_add_popup_btn_cancel,
                                  edit);
   elm_object_part_content_set(edit->popup, "button2", bt_no);

   evas_object_show(edit->popup);
   elm_object_focus_set(edit->entry, true);
}

static void
_on_button_delete_clicked_cb(void *data __UNUSED__,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);

   Uns_List *colorclass = NULL;
   Colorclass_Item *cc_it = NULL;
   Eina_List *l;

   if (!edit->current_ccl) return;

   /* check if this is newly added */
   EINA_LIST_FOREACH(edit->unapplied_list, l, colorclass)
     {
        cc_it = (Colorclass_Item *)colorclass->data;

        if ((colorclass->act_type == ACTION_TYPE_ADD) &&
            (!strcmp(edit->current_ccl->name, cc_it->name)))
          {
             edit->unapplied_list = eina_list_remove_list(edit->unapplied_list, l);
             eina_stringshare_del(cc_it->name);
             free(cc_it);
             free(colorclass);
             cc_it = NULL;
             colorclass = NULL;
             break;
          }
        cc_it = NULL;
        colorclass = NULL;
     }

   TODO("Check if colorclass is exist in project, if not, no need to delete it.");
   cc_it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   cc_it->name = eina_stringshare_add(edit->current_ccl->name);

   colorclass = mem_malloc(sizeof(Uns_List));
   colorclass->data = cc_it;
   colorclass->act_type = ACTION_TYPE_DEL;
   edit->unapplied_list = eina_list_append(edit->unapplied_list, colorclass);

   Elm_Object_Item *it = elm_genlist_selected_item_get(edit->genlist);
   Elm_Object_Item *next = elm_genlist_item_next_get(it);

   TODO("Fix me")
//   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get());
//   ui_property_state_unset(ui_block_property_get());
//   ui_property_state_set(ui_block_property_get(), part);

   if (!next) next = elm_genlist_item_prev_get(it);
   if (next)
      elm_genlist_item_selected_set(next, EINA_TRUE);
   else
      edit->current_ccl = NULL;

   elm_object_item_del(it);

   if (elm_genlist_items_count(edit->genlist) == 0)
     {
        elm_object_disabled_set(edit->colorsel1, true);
        elm_object_disabled_set(edit->colorsel2, true);
        elm_object_disabled_set(edit->colorsel3, true);
     }
}

/* Changing background of preview */
static void
_change_bg_cb(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);

   int state = elm_radio_state_value_get(obj);
   Evas_Object *bg = elm_object_part_content_unset(edit->layout, "swallow.entry.bg");
   evas_object_del(bg);
   Evas *canvas = evas_object_evas_get(obj);
   switch (state)
     {
      case 0:
        {
           IMAGE_ADD_NEW(edit->layout, bg, "bg", "tile");
           evas_object_show(bg);
        }
      break;
      case 1:
        {
           bg = evas_object_rectangle_add(canvas);
           evas_object_color_set(bg, 0, 0, 0, 255);
           evas_object_show(bg);
        }
      break;
      case 2:
        {
           bg = evas_object_rectangle_add(canvas);
           evas_object_color_set(bg, 255, 255, 255, 255);
           evas_object_show(bg);
        }
      break;
      default:
        {
           ERR("Wrong state");
           abort();
        }
      break;
     }

   elm_object_part_content_set(edit->layout, "swallow.entry.bg", bg);
}

static void
_colorclass_update(Colorclasses_Manager *edit)
{
   assert(edit != NULL);

   edje_object_color_class_set(edit->edje_preview,
                               "colorclass_manager/text_example_colorclass",
                               edit->current_ccl->r1, edit->current_ccl->g1,
                               edit->current_ccl->b1, edit->current_ccl->a1,
                               edit->current_ccl->r2, edit->current_ccl->g2,
                               edit->current_ccl->b2, edit->current_ccl->a2,
                               edit->current_ccl->r3, edit->current_ccl->g3,
                               edit->current_ccl->b3, edit->current_ccl->a3);

   /* check if this is newly added */
   Eina_List *l;
   Uns_List *colorclass = NULL;
   Colorclass_Item *cc_it = NULL;
   EINA_LIST_FOREACH(edit->unapplied_list, l, colorclass)
     {
        cc_it = (Colorclass_Item *)colorclass->data;

        if ((colorclass->act_type == ACTION_TYPE_ADD) &&
            (!strcmp(edit->current_ccl->name, cc_it->name)))
          {
             cc_it->r1 = edit->current_ccl->r1; cc_it->g1 = edit->current_ccl->g1;
             cc_it->b1 = edit->current_ccl->b1; cc_it->a1 = edit->current_ccl->a1;
             cc_it->r2 = edit->current_ccl->r2; cc_it->g2 = edit->current_ccl->g2;
             cc_it->b2 = edit->current_ccl->b2; cc_it->a2 = edit->current_ccl->a2;
             cc_it->r3 = edit->current_ccl->r3; cc_it->g3 = edit->current_ccl->g3;
             cc_it->b3 = edit->current_ccl->b3; cc_it->a3 = edit->current_ccl->a3;
             return;
          }
        else if ((colorclass->act_type == ACTION_TYPE_DEL) &&
                 (!strcmp(edit->current_ccl->name, cc_it->name)))
          return;
     }

   cc_it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   cc_it->name = eina_stringshare_add(edit->current_ccl->name);

   cc_it->r1 = edit->current_ccl->r1; cc_it->g1 =edit->current_ccl->g1;
   cc_it->b1 = edit->current_ccl->b1; cc_it->a1 =edit->current_ccl->a1;
   cc_it->r2 = edit->current_ccl->r2; cc_it->g2 =edit->current_ccl->g2;
   cc_it->b2 = edit->current_ccl->b2; cc_it->a2 =edit->current_ccl->a2;
   cc_it->r3 = edit->current_ccl->r3; cc_it->g3 =edit->current_ccl->g3;
   cc_it->b3 = edit->current_ccl->b3; cc_it->a3 =edit->current_ccl->a3;

   colorclass = mem_malloc(sizeof(Uns_List));
   colorclass->data = (void *)cc_it;
   colorclass->act_type = ACTION_TYPE_ADD;
   edit->unapplied_list = eina_list_append(edit->unapplied_list, colorclass);
}
/* Colorselector widget callbacks */
#define COLORSELECTOR_CALLBACK(NUMBER) \
static void \
_on_changed_##NUMBER(void *data, \
                     Evas_Object *obj __UNUSED__, \
                     void *event_info __UNUSED__) \
{ \
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data; \
   assert(edit != NULL); \
   if (!edit->current_ccl) return; \
   elm_colorselector_color_get(edit->colorsel##NUMBER, \
                               &edit->current_ccl->r##NUMBER, \
                               &edit->current_ccl->g##NUMBER, \
                               &edit->current_ccl->b##NUMBER, \
                               &edit->current_ccl->a##NUMBER); \
   _colorclass_update(edit); \
   edit->changed = true; \
}

COLORSELECTOR_CALLBACK(1)
COLORSELECTOR_CALLBACK(2)
COLORSELECTOR_CALLBACK(3)

#undef COLORSELECTOR_CALLBACK

/* Callback on colorclass selection in list */
static void
_on_selected(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

   assert(edit != NULL);

   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Colorclass_Item *ccl = elm_object_item_data_get(glit);

   assert(ccl != NULL);

   edit->current_ccl = ccl;

   elm_colorselector_color_set(edit->colorsel1, ccl->r1, ccl->g1, ccl->b1, ccl->a1);
   elm_colorselector_color_set(edit->colorsel2, ccl->r2, ccl->g2, ccl->b2, ccl->a2);
   elm_colorselector_color_set(edit->colorsel3, ccl->r3, ccl->g3, ccl->b3, ccl->a3);

   edje_object_color_class_set(edit->edje_preview,
                               "colorclass_manager/text_example_colorclass",
                               ccl->r1, ccl->g1,
                               ccl->b1, ccl->a1,
                               ccl->r2, ccl->g2,
                               ccl->b2, ccl->a2,
                               ccl->r3, ccl->g3,
                               ccl->b3, ccl->a3);

   elm_object_disabled_set(edit->colorsel1, false);
   elm_object_disabled_set(edit->colorsel2, false);
   elm_object_disabled_set(edit->colorsel3, false);
}

/* Modal Window callbacks (closing and exiting from colorclass manager) */
static void
_on_colorclass_editor_del(void * data,
                          Evas *e __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;
   Eina_List *l;
   Uns_List *it = NULL;
   Colorclass_Item *ccl_it = NULL;

   assert(edit != NULL);

   EINA_LIST_FOREACH(edit->unapplied_list, l, it)
     {
        ccl_it = (Colorclass_Item *)it->data;

        if (it->act_type == ACTION_TYPE_DEL)
          edje_edit_color_class_del(ap.project->global_object, ccl_it->name);
        else
          {
             edje_edit_color_class_add(ap.project->global_object, eina_stringshare_add(ccl_it->name));
             edje_edit_color_class_colors_set(ap.project->global_object, ccl_it->name,
                                              ccl_it->r1, ccl_it->g1,
                                              ccl_it->b1, ccl_it->a1,
                                              ccl_it->r2, ccl_it->g2,
                                              ccl_it->b2, ccl_it->a2,
                                              ccl_it->r3, ccl_it->g3,
                                              ccl_it->b3, ccl_it->a3);
          }
        eina_stringshare_del(ccl_it->name);
        free(ccl_it);
        ccl_it = NULL;
     }

   eina_list_free(edit->unapplied_list);
}

/* Search functions and creatings */
ITEM_SEARCH_FUNC(genlist,ELM_GENLIST_ITEM_SCROLLTO_MIDDLE, "elm.text")
static inline Evas_Object *
_manager_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
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
   Colorclasses_Manager *edit = data;

   assert(edit != NULL);

   _genlist_item_search(edit->genlist, &(edit->style_search_data),
                        edit->style_search_data.last_item_found);
}
static void
_search_nxt_gd_item(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = data;
   Elm_Object_Item *start_from = NULL;

   assert(edit != NULL);

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

   assert(search_data != NULL);

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
_colorclass_main_layout_create(Colorclasses_Manager *edit)
{
   Evas_Object *search, *bg, *box_bg, *radio, *radio_group, *image_bg, *ic, *button;

   assert(edit != NULL);

   /* Creating main layout of window */
   edit->layout = elm_layout_add(ap.win);
   evas_object_event_callback_add(edit->layout, EVAS_CALLBACK_DEL,
                                  _on_colorclass_editor_del, edit);
   elm_layout_theme_set(edit->layout, "layout", "colorclass_manager", "default");
   evas_object_size_hint_weight_set(edit->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   /* List of project's colorclasses */
   edit->genlist = elm_genlist_add(edit->layout);
   evas_object_size_hint_weight_set(edit->genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(edit->genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(edit->genlist);
   elm_object_part_content_set(edit->layout, "swallow.list", edit->genlist);
   evas_object_smart_callback_add(edit->genlist, "selected", _on_selected, edit);

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = "default";
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.del = _item_ccl_del;
     }

   /* Search engine */
   search = _manager_search_field_create(edit->layout);
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

   edit->edje_preview = edje_object_add(evas_object_evas_get(ap.win));
   if (!edje_object_file_set(edit->edje_preview,
                             EFLETE_THEME,
                             "elm/layout/colorclass_manager/preview"))
     {
        ERR("Couldn't load layout for text example field!");
        abort();
     }
   edje_object_part_text_set(edit->edje_preview, "text_example",
                       _("The quick brown fox jumps over the lazy dog"));
   evas_object_size_hint_align_set(edit->edje_preview, -1, -1);
   evas_object_show(edit->edje_preview);
   elm_object_part_content_set(edit->layout, "swallow.entry", edit->edje_preview);

   /* Creating colorselectors */
#define ADD_COLORSEL(NUMBER, SWALLOW_NAME, COLORSEL_NAME) \
   edit->colorsel##NUMBER = elm_colorselector_add(edit->layout); \
   elm_colorselector_mode_set(edit->colorsel##NUMBER, ELM_COLORSELECTOR_ALL); \
   elm_object_part_content_set(edit->layout, "swallow.colorselector."SWALLOW_NAME, edit->colorsel##NUMBER); \
   elm_object_part_text_set(edit->layout, "text."SWALLOW_NAME, COLORSEL_NAME); \
   evas_object_smart_callback_add(edit->colorsel##NUMBER, "changed,user", _on_changed_##NUMBER, edit); \
   evas_object_smart_callback_add(edit->colorsel##NUMBER, "color,item,selected", _on_changed_##NUMBER, edit); \
   elm_object_disabled_set(edit->colorsel##NUMBER, true);

   ADD_COLORSEL(1, "object", _("Object color"));
   ADD_COLORSEL(2, "outline", _("Outline color"));
   ADD_COLORSEL(3, "shadow", _("Shadow color"));

#undef ADD_COLORSEL


   /* Background changing radios */
   BOX_ADD(edit->layout, box_bg, true, false);
   elm_box_padding_set(box_bg, 10, 0);

#define _RADIO_ADD(RADIO, VALUE, IMAGE) \
   RADIO = elm_radio_add(edit->layout); \
   elm_object_style_set(RADIO, "style_editor"); \
   elm_radio_state_value_set(RADIO, VALUE); \
   evas_object_show(RADIO); \
   IMAGE_ADD_NEW(box_bg, image_bg, "preview", IMAGE); \
   elm_object_part_content_set(RADIO, "bg", image_bg); \
   evas_object_smart_callback_add(RADIO, "changed", _change_bg_cb, edit); \
   elm_box_pack_end(box_bg, RADIO);

   _RADIO_ADD(radio_group, 0, "bg-tile");
   _RADIO_ADD(radio, 1, "bg-black");
   elm_radio_group_add(radio, radio_group);
   _RADIO_ADD(radio, 2, "bg-white");
   elm_radio_group_add(radio, radio_group);
#undef _RADIO_ADD

   elm_object_part_content_set(edit->layout, "swallow.radio", box_bg);

   /* Controls (add, remove) of colorclasses */
   button = elm_button_add(edit->layout);
   evas_object_show(button);
   ICON_STANDARD_ADD(button, ic, true, "plus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, edit);
   elm_object_part_content_set(edit->layout, "swallow.control.add", button);

   button = elm_button_add(edit->layout);
   evas_object_show(button);
   ICON_STANDARD_ADD(button, ic, true, "minus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_delete_clicked_cb, edit);
   elm_object_part_content_set(edit->layout, "swallow.control.minus", button);
   elm_object_disabled_set(button, true);
}
Eina_Bool
_colorclass_manager_init(Colorclasses_Manager *edit)
{
   Eina_List *l = NULL;
   Colorclass_Item *it = NULL;
   Colorclass_Resource *res;

   assert(edit != NULL);
   assert(ap.project != NULL);

   EINA_LIST_FOREACH(ap.project->colorclasses, l, res)
     {
        it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));

        it->r1 = res->color1.r; it->g1 = res->color1.g;
        it->b1 = res->color1.b; it->a1 = res->color1.a;
        it->r2 = res->color2.r; it->g2 = res->color2.g;
        it->b2 = res->color2.b; it->a2 = res->color2.a;
        it->r3 = res->color3.r; it->g3 = res->color3.g;
        it->b3 = res->color3.b; it->a3 = res->color3.a;

        it->name = eina_stringshare_add(res->name);
        elm_genlist_item_append(edit->genlist, _itc_ccl, it, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   return true;
}

Evas_Object *
colorclass_manager_add()
{
   Colorclasses_Manager *edit = NULL;

   assert(ap.project != NULL);

   edit = (Colorclasses_Manager *)mem_calloc(1, sizeof(Colorclasses_Manager));
   edit->changed = false;
   assert(ap.win != NULL);

   _colorclass_main_layout_create(edit);
   if (!_colorclass_manager_init(edit))
     {
        ERR(_("Failed initialize colorclasses manager"));
        abort();
     }

   return edit->layout;
}
