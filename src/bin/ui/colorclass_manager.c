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

#include "main_window.h"
#include "project_manager.h"

static Elm_Genlist_Item_Class *_itc_ccl = NULL;

typedef struct _Colorclasses_Manager Colorclasses_Manager;
typedef struct _Search_Data Search_Data;

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Colorclasses_Manager
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *edje_preview, *preview_layout;
   Evas_Object *entry, *popup;
   Evas_Object *btn_add;
   Elm_Validator_Regexp *name_validator;
   Search_Data style_search_data;
   Colorclass_Item *current_ccl;
};

/* BUTTON ADD AND REMOVE FUNCTIONS */
static void
_validation(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
  Colorclasses_Manager *edit = (Colorclasses_Manager *)data;

  if (ELM_REG_NOERROR != elm_validator_regexp_status_get(edit->name_validator))
    popup_buttons_disabled_set(BTN_OK, true);
  else
    popup_buttons_disabled_set(BTN_OK, false);
}

static void
_on_button_add_clicked_cb(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Evas_Object *box, *item;
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;
   Colorclass_Item *it = NULL;
   Elm_Object_Item *glit_ccl = NULL;
   Colorclass_Resource *res;
   Popup_Button btn_res;

   assert(edit != NULL);

   box = elm_box_add(ap.win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

   edit->name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   LAYOUT_PROP_ADD(box, _("Color class name: "), "property", "1swallow")
   ENTRY_ADD(box, edit->entry, true);
   eo_do(edit->entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, edit->name_validator));
   evas_object_smart_callback_add(edit->entry, "changed", _validation, edit);
   elm_object_part_text_set(edit->entry, "guide", _("Type new color class name here"));
   elm_object_part_content_set(item, "elm.swallow.content", edit->entry);

   elm_box_pack_end(box, item);
   elm_box_pack_end(box, edit->entry);

   btn_res = popup_want_action(_("Create a new layout"), NULL, box,
                               edit->entry, BTN_OK|BTN_CANCEL,
                               NULL, NULL);

   if (BTN_CANCEL == btn_res) goto end;

   it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   it->name = elm_entry_entry_get(edit->entry);

   res = mem_calloc(1, sizeof(Colorclass_Resource));
   res->name = eina_stringshare_add(it->name);
   ap.project->colorclasses = eina_list_sorted_insert(ap.project->colorclasses,
                                                      (Eina_Compare_Cb) resource_cmp,
                                                      res);
   edje_edit_color_class_add(ap.project->global_object, eina_stringshare_add(it->name));

   glit_ccl = elm_genlist_item_append(edit->genlist, _itc_ccl, it, NULL,
                                      ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_ccl, EINA_TRUE);

   evas_object_del(edit->popup);
   edit->popup = NULL;

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;

end:
   evas_object_del(box);
}

static void
_on_button_delete_clicked_cb(void *data __UNUSED__,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;
   State *state;
   char buf[BUFF_MAX];
   int symbs = 0;
   Eina_List *l;
   Resource *res;

   assert(edit != NULL);

   if (!edit->current_ccl) return;

   Elm_Object_Item *it = elm_genlist_selected_item_get(edit->genlist);
   Elm_Object_Item *next = elm_genlist_item_next_get(it);
   Colorclass_Item *ccl = elm_object_item_data_get(it);

   res = pm_resource_get(ap.project->colorclasses, ccl->name);
   if (!res->used_in)
     {
        edje_edit_color_class_del(ap.project->global_object, ccl->name);
        ap.project->colorclasses = pm_resource_del(ap.project->colorclasses, res);
        elm_object_item_del(it);
     }
   else
     {
        snprintf(buf, BUFF_MAX, _("Colorclass is used in:"));
        symbs = strlen(buf);
        EINA_LIST_FOREACH(res->used_in, l, state)
          {
             snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>group: %s<br>part: %s<br>state: \"%s\" %2.1f"),
                      state->part->group->name, state->part->name, state->parsed_name, state->parsed_val);
             symbs += strlen(res->name);
             break; TODO("remove this break after warning style remake")
          }
        WARN("%s", buf);
        return;
     }

   if (!next) next = elm_genlist_item_prev_get(it);
   if (next)
      elm_genlist_item_selected_set(next, EINA_TRUE);
   else
      edit->current_ccl = NULL;

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
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
   Evas_Object *preview_layout = elm_object_part_content_get(edit->layout, "swallow.preview");
   Evas_Object *bg = elm_object_part_content_unset(preview_layout, "swallow.entry.bg");
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

   elm_object_part_content_set(preview_layout, "swallow.entry.bg", bg);
}

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

   ColorClassData *current_color_data = (ColorClassData *)mem_malloc(sizeof(ColorClassData));
   memset(current_color_data, 0x0, sizeof(ColorClassData));

   edit->current_ccl = ccl;

   edje_object_color_class_set(edit->edje_preview,
                               "colorclass_manager/text_example_colorclass",
                               ccl->r1, ccl->g1,
                               ccl->b1, ccl->a1,
                               ccl->r2, ccl->g2,
                               ccl->b2, ccl->a2,
                               ccl->r3, ccl->g3,
                               ccl->b3, ccl->a3);

   current_color_data->current_ccl = ccl;
   current_color_data->edje_preview = edit->edje_preview;
   evas_object_smart_callback_call(ap.win, SIGNAL_COLOR_SELECTED, current_color_data);
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

static Evas_Object *
_form_list_side(Colorclasses_Manager *edit)
{
   Evas_Object *layout, *search, *ic, *button;

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = "default";
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.del = _item_ccl_del;
     }

   layout = elm_layout_add(ap.win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "style_color_editor", "list");
   elm_object_part_text_set(layout, "label.list", _("Color list"));
   evas_object_show(layout);

   /* List of project's colorclasses */
   edit->genlist = elm_genlist_add(layout);
   evas_object_size_hint_weight_set(edit->genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(edit->genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(edit->genlist);
   elm_object_part_content_set(layout, "swallow.list", edit->genlist);
   evas_object_smart_callback_add(edit->genlist, "selected", _on_selected, edit);

   search = _manager_search_field_create(layout);
   elm_object_part_content_set(layout, "swallow.search", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, edit);
   evas_object_smart_callback_add(edit->genlist, "pressed", _search_reset_cb,
                                  &(edit->style_search_data));
   edit->style_search_data.search_entry = search;
   edit->style_search_data.last_item_found = NULL;

   /* Controls (add, remove) of colorclasses */
   button = elm_button_add(layout);
   evas_object_show(button);
   ICON_STANDARD_ADD(button, ic, true, "plus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, edit);
   elm_object_part_content_set(layout, "swallow.button_add", button);

   button = elm_button_add(layout);
   evas_object_show(button);
   ICON_STANDARD_ADD(button, ic, true, "minus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_delete_clicked_cb, edit);
   elm_object_part_content_set(layout, "swallow.button_rm", button);

   return layout;
}

static void
_colorclass_main_layout_create(Colorclasses_Manager *edit)
{
   Evas_Object *layout_list, *bg, *box_bg, *radio, *radio_group, *image_bg, *layout_bg, *btn_apply;

   assert(edit != NULL);

   /* Creating main layout of window */
   edit->layout = elm_layout_add(ap.win);
   elm_layout_theme_set(edit->layout, "layout", "tab_style", "style_color_editor");
   evas_object_size_hint_weight_set(edit->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   elm_object_part_text_set(edit->layout, "label.preview", _("Preview"));

   layout_list = _form_list_side(edit);
   elm_object_part_content_set(edit->layout, "swallow.list", layout_list);
   evas_object_show(layout_list);


   layout_bg = elm_layout_add(edit->layout);
   elm_layout_theme_set(layout_bg, "layout", "colorclass_editor", "preview");
   evas_object_size_hint_weight_set(layout_bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   /* Entry preview to show colorclass */
   IMAGE_ADD_NEW(layout_bg, bg, "bg", "tile");
   elm_object_part_content_set(layout_bg, "swallow.entry.bg", bg);
   evas_object_show(bg);

   edit->edje_preview = edje_object_add(evas_object_evas_get(layout_bg));
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
   elm_object_part_content_set(layout_bg, "swallow.entry", edit->edje_preview);

   elm_object_part_content_set(edit->layout, "swallow.preview", layout_bg);

   /* Background changing radios */
   BOX_ADD(edit->layout, box_bg, true, false);
   elm_box_padding_set(box_bg, 10, 0);
   elm_box_align_set(box_bg, 1.0, 0.5);

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

   elm_object_part_content_set(edit->layout, "menu_container", box_bg);

   BUTTON_ADD(edit->layout, btn_apply, _("Apply"))
   elm_object_part_content_set(edit->layout, "elm.swallow.btn_apply", btn_apply);
   elm_object_disabled_set(btn_apply, EINA_TRUE);
}

static Eina_Bool
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
   assert(ap.win != NULL);

   _colorclass_main_layout_create(edit);
   if (!_colorclass_manager_init(edit))
     {
        ERR(_("Failed initialize colorclasses manager"));
        abort();
     }

   return edit->layout;
}
