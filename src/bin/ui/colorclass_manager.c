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
#include "validator.h"
#include "project_manager.h"
#include "modal_window.h"

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
   Evas_Object *win;
   Evas_Object *panes;
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *edje_preview, *preview_layout;
   Evas_Object *entry, *popup;
   Evas_Object *del_button;
   Evas_Object *item;
   Resource_Name_Validator *name_validator;
   Search_Data style_search_data;
   Colorclass_Item *current_ccl;
   struct {
        Evas_Object *black;
        Evas_Object *tile;
        Evas_Object *white;
   } bg_switcher;
};

static Colorclasses_Manager mng;

static void
_validation(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
  if (ELM_REG_NOERROR != resource_name_validator_status_get(mng.name_validator))
    popup_buttons_disabled_set(BTN_OK, true);
  else
    popup_buttons_disabled_set(BTN_OK, false);
}

Evas_Object *
_add_colorclass_content_get(void *data __UNUSED__, Evas_Object **to_focus)
{
   Evas_Object *item = NULL;

   LAYOUT_PROP_ADD(ap.win, _("Color class name: "), "property", "1swallow")
   ENTRY_ADD(item, mng.entry, true);
   eo_event_callback_add(mng.entry, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, mng.name_validator);
   evas_object_smart_callback_add(mng.entry, "changed", _validation, NULL);
   elm_object_part_text_set(mng.entry, "guide", _("Type new color class name here"));
   elm_object_part_content_set(item, "elm.swallow.content", mng.entry);
   mng.item = item;
   if (to_focus) *to_focus = mng.entry;
   popup_buttons_disabled_set(BTN_OK, true);

   return mng.item;
}

static void
_colorclass_add_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Colorclasses_Manager *edit = (Colorclasses_Manager *)data;
   Colorclass_Item *it = NULL;
   Elm_Object_Item *glit_ccl = NULL;
   Colorclass_Resource *res;
   Popup_Button btn_res;

   assert(edit != NULL);

   mng.name_validator = resource_name_validator_new(NAME_REGEX, NULL);
   resource_name_validator_list_set(mng.name_validator, &ap.project->colorclasses, true);
   btn_res = popup_want_action(_("Create a new layout"), NULL, _add_colorclass_content_get,
                               BTN_OK|BTN_CANCEL,
                               NULL, mng.entry);

   if (BTN_CANCEL == btn_res) goto end;

   it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   it->name = elm_entry_entry_get(mng.entry);

   res = (Colorclass_Resource *)resource_add(it->name, RESOURCE_TYPE_COLORCLASS);
   resource_insert(&ap.project->colorclasses, (Resource *)res);
   edje_edit_color_class_add(ap.project->global_object, eina_stringshare_add(it->name));

   glit_ccl = elm_genlist_item_append(mng.genlist, _itc_ccl, it, NULL,
                                      ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_ccl, EINA_TRUE);

   evas_object_del(mng.popup);
   mng.popup = NULL;

   CRIT_ON_FAIL(editor_save(ap.project->global_object));
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;

end:
   resource_name_validator_free(mng.name_validator);
   evas_object_del(mng.item);
}

static void
_colorclass_del_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Resource *res;
   Resource request;

   Elm_Object_Item *it = elm_genlist_selected_item_get(mng.genlist);
   Elm_Object_Item *next = elm_genlist_item_next_get(it);
   Colorclass_Item *ccl = elm_object_item_data_get(it);

   request.resource_type = RESOURCE_TYPE_COLORCLASS;
   request.name = ccl->name;
   res = resource_get(ap.project->colorclasses, &request);
   edje_edit_color_class_del(ap.project->global_object, ccl->name);
   resource_remove(&ap.project->colorclasses, res);
   resource_free(res);
   elm_object_item_del(it);

#if 0
   State *state;
   char buf[BUFF_MAX];
   int symbs = 0;
   Eina_List *l;
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
#endif

   if (!next) next = elm_genlist_item_prev_get(it);
   if (next)
      elm_genlist_item_selected_set(next, EINA_TRUE);
   else
     {
        mng.current_ccl = NULL;
        elm_object_disabled_set(mng.del_button, EINA_TRUE);
        evas_object_smart_callback_call(ap.win, SIGNAL_COLOR_SELECTED, NULL);
     }

   CRIT_ON_FAIL(editor_save(ap.project->global_object));
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
}

/* Callback on colorclass (un)selection in list */
static void
_on_unselected(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   elm_object_disabled_set(mng.del_button, EINA_TRUE);
   evas_object_smart_callback_call(ap.win, SIGNAL_COLOR_SELECTED, NULL);
}

static void
_on_selected(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Colorclass_Item *ccl = elm_object_item_data_get(glit);
   elm_object_disabled_set(mng.del_button, EINA_FALSE);

   assert(ccl != NULL);

   ColorClassData *current_color_data = (ColorClassData *)mem_calloc(1, sizeof(ColorClassData));

   mng.current_ccl = ccl;

   edje_object_color_class_set(mng.edje_preview,
                               "colorclass_manager/text_example_colorclass",
                               ccl->r1, ccl->g1,
                               ccl->b1, ccl->a1,
                               ccl->r2, ccl->g2,
                               ccl->b2, ccl->a2,
                               ccl->r3, ccl->g3,
                               ccl->b3, ccl->a3);

   current_color_data->current_ccl = ccl;
   current_color_data->edje_preview = mng.edje_preview;
   elm_object_signal_emit(mng.preview_layout, "entry,show", "eflete");
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
_search_changed(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   _genlist_item_search(mng.genlist, &(mng.style_search_data),
                        mng.style_search_data.last_item_found);
}
static void
_search_nxt_gd_item(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Elm_Object_Item *start_from = NULL;

   if (mng.style_search_data.last_item_found)
     start_from = elm_genlist_item_next_get(mng.style_search_data.last_item_found);

   _genlist_item_search(mng.genlist, &(mng.style_search_data), start_from);
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

#ifndef HAVE_TIZEN
static void
_bg_cb(void *data,
       Evas_Object *obj,
       void *event_info __UNUSED__)
{
   Bg_Preview bg_mode;
   const char *signal = NULL;
   Evas_Object *entry_prev = data;

   Evas_Object *bg = elm_object_part_content_get(entry_prev, "elm.swallow.background");
   bg_mode = elm_radio_value_get(obj);
   switch (bg_mode)
     {
      case BG_PREVIEW_WHITE:
         signal = "elm,bg,white";
         break;
      case BG_PREVIEW_TILE:
         signal = "elm,bg,tile";
         break;
      case BG_PREVIEW_BLACK:
         signal = "elm,bg,black";
         break;
     }

   elm_layout_signal_emit(bg, signal, "eflete");
}

static Evas_Object *
_radio_switcher_add(Evas_Object *obj,
                    const char *style,
                    Evas_Smart_Cb func,
                    int state_value,
                    Evas_Object *group)
{
   Evas_Object *radio;

   radio = elm_radio_add(ap.win);
   evas_object_show(radio);
   elm_object_style_set(radio, style);
   elm_radio_state_value_set(radio, state_value);
   evas_object_smart_callback_add(radio, "changed", func, obj);
   elm_radio_group_add(radio, group);

   return radio;
}
#endif

static void
_mw_cancel_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *content;

   /* unset and hide the image manager content */
   content = elm_object_content_unset(mng.win);
   evas_object_hide(content);
}

static void
_mw_done_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Evas_Object *content;

   /* unset and hide the image manager content */
   content = elm_object_content_unset(mng.win);
   evas_object_hide(content);
}

static Eina_Bool
_colorclass_manager_init(void)
{
   Eina_List *l = NULL;
   Colorclass_Item *it = NULL;
   Colorclass_Resource *res;

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
        elm_genlist_item_append(mng.genlist, _itc_ccl, it, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_COLOR_SELECTED, NULL);
   return true;
}

Evas_Object *
colorclass_manager_add(void)
{
   Evas_Object *bg, *box_bg, *search, *ic, *button;

   assert(ap.project != NULL);

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = "default";
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.del = _item_ccl_del;
     }

   /* Creating main layout of window */
   mng.win = mw_add();
   mw_title_set(mng.win, _("Color class manager"));
   evas_object_smart_callback_add(mng.win, "cancel", _mw_cancel_cb, NULL);
   evas_object_smart_callback_add(mng.win, "done", _mw_done_cb, NULL);
   ic = elm_icon_add(mng.win);
   elm_icon_standard_set(ic, "color");
   mw_icon_set(mng.win, ic);
   mng.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(mng.layout, "layout", "manager", "internal");
   elm_layout_text_set(mng.layout, "elm.text", _("Preview"));
   elm_layout_text_set(mng.layout, "elm.subtext", _("Color classes list"));
   mng.panes = elm_panes_add(mng.win);
   elm_panes_content_right_size_set(mng.panes, 0);
   elm_panes_content_right_min_size_set(mng.panes, 355);
   elm_object_content_set(mng.win, mng.panes);
   elm_object_part_content_set(mng.panes, "left", mng.layout);
   elm_object_part_content_set(mng.panes, "right", ap.property.color_manager);

   mng.genlist = elm_genlist_add(mng.layout);
#ifdef HAVE_TIZEN
   elm_object_style_set(mng.panes, "manager");
   elm_object_style_set(mng.genlist, "manager");
#endif
   evas_object_show(mng.genlist);
   elm_object_part_content_set(mng.layout, "elm.swallow.list", mng.genlist);
   evas_object_smart_callback_add(mng.genlist, "selected", _on_selected, NULL);
   evas_object_smart_callback_add(mng.genlist, "unselected", _on_unselected, NULL);

   search = _manager_search_field_create(mng.layout);
   elm_object_part_content_set(mng.layout, "elm.swallow.search", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, NULL);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, NULL);
   evas_object_smart_callback_add(mng.genlist, "pressed", _search_reset_cb, &(mng.style_search_data));
   mng.style_search_data.search_entry = search;
   mng.style_search_data.last_item_found = NULL;

   /* Controls (add, remove) of colorclasses */
   button = elm_button_add(mng.layout);
   elm_object_style_set(button, "plus_managers");
   evas_object_smart_callback_add(button, "clicked", _colorclass_add_cb, &mng);
   elm_object_part_content_set(mng.layout, "elm.swallow.btn_add", button);

   mng.del_button = elm_button_add(mng.layout);
   elm_object_style_set(mng.del_button, "minus_managers");
   evas_object_smart_callback_add(mng.del_button, "clicked", _colorclass_del_cb, NULL);
   elm_object_part_content_set(mng.layout, "elm.swallow.btn_del", mng.del_button);
   elm_object_disabled_set(mng.del_button, EINA_TRUE);

   mng.preview_layout = elm_layout_add(mng.layout);
   elm_layout_theme_set(mng.preview_layout, "layout", "manager", "preview");
   elm_object_signal_emit(mng.preview_layout, "entry,hide", "eflete");

   /* Entry preview to show colorclass */
   bg = elm_layout_add(mng.preview_layout);
   elm_layout_theme_set(bg, "layout", "workspace", "bg");
   elm_object_part_content_set(mng.preview_layout, "elm.swallow.background", bg);
   evas_object_show(bg);

   mng.edje_preview = edje_object_add(evas_object_evas_get(mng.preview_layout));
   if (!edje_object_file_set(mng.edje_preview, EFLETE_THEME,
                             "elm/layout/colorclass_manager/preview"))
     {
        ERR("Couldn't load layout for text example field!");
        abort();
     }
   edje_object_part_text_set(mng.edje_preview, "text_example",
                       _("The quick brown fox jumps over the lazy dog"));
   evas_object_size_hint_align_set(mng.edje_preview, -1, -1);
   evas_object_show(mng.edje_preview);
   elm_object_part_content_set(mng.preview_layout, "elm.swallow.entry", mng.edje_preview);

   elm_object_part_content_set(mng.layout, "elm.swallow.preview", mng.preview_layout);

   /* Background changing radios */
   BOX_ADD(mng.layout, box_bg, true, false);
   elm_box_padding_set(box_bg, 6, 0);
   elm_box_align_set(box_bg, 1.0, 0.5);

   /* add to toolbar bg switcher */
#ifndef HAVE_TIZEN
   mng.bg_switcher.white = _radio_switcher_add(mng.preview_layout, "bg_white", _bg_cb, BG_PREVIEW_WHITE, NULL);
   elm_box_pack_end(box_bg, mng.bg_switcher.white);

   mng.bg_switcher.tile = _radio_switcher_add(mng.preview_layout, "bg_tile", _bg_cb, BG_PREVIEW_TILE, mng.bg_switcher.white);
   elm_box_pack_end(box_bg, mng.bg_switcher.tile);

   mng.bg_switcher.black = _radio_switcher_add(mng.preview_layout, "bg_black", _bg_cb, BG_PREVIEW_BLACK, mng.bg_switcher.white);
   elm_box_pack_end(box_bg, mng.bg_switcher.black);

   elm_radio_value_set(mng.bg_switcher.white, BG_PREVIEW_TILE);

   elm_object_part_content_set(mng.layout, "elm.swallow.menu", box_bg);
#endif

   if (!_colorclass_manager_init())
     {
        ERR(_("Failed initialize colorclasses manager"));
        abort();
     }

   evas_object_show(mng.win);
   return mng.win;
}
