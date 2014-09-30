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

#include "sound_editor.h"
#include "main_window.h"

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define SND_EDIT_KEY "sound_editor_key"

typedef struct _Sound_Editor Sound_Editor;
typedef struct _Search_Data Search_Data;
typedef struct _Item Item;

struct _Item
{
   const char* sound_name;
   Edje_Edit_Sound_Comp comp;
   int tone_frq;
};

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Sound_Editor
{
   Project *pr;
   Evas_Object *win;
   Evas_Object *gengrid;
   Evas_Object *player;
   Evas_Object *sample_box;
   Evas_Object *tone_box;
   Evas_Object *markup;
   Search_Data sound_search_data;
   struct {
      Evas_Object *tone_name;
      Evas_Object *tone_frq;
      Evas_Object *file_name;
      Evas_Object *type;
      Evas_Object *size;
      Evas_Object *comp;
      Evas_Object *quality;
   } snd_data;
};

static Elm_Gengrid_Item_Class *gic = NULL, *ggic = NULL;

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Item *it = data;
   return strdup(it->sound_name);
}

static void
_sound_editor_del(Sound_Editor *edit)
{
   edit->pr = NULL;
   elm_gengrid_item_class_free(gic);
   elm_gengrid_item_class_free(ggic);
   evas_object_del(edit->win);
   evas_object_del(edit->gengrid);
   evas_object_del(edit->markup);
   free(edit);
}

static void
_on_quit_cb(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;
   _sound_editor_del(edit);
}

static void
_on_ok_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
  // Sound_Editor *edit = (Sound_Editor *)data;
   /*It will be implemented*/
}

static Evas_Object *
_grid_content_get(void *data __UNUSED__,
                  Evas_Object *obj,
                  const char  *part)
{
   Evas_Object *image = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        GET_IMAGE(image, obj, "sound");
        evas_object_show(image);
        return image;
     }
   return NULL;
}

static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;
   eina_stringshare_del(it->sound_name);
   free(it);
}

static Evas_Object *
_sound_info_entry_add(Evas_Object *box,
                      const char *label)
{
   Evas_Object *item, *entry;
   ITEM_ADD(box, item, label, "eflete/image_editor/item/default");
   EWE_ENTRY_ADD(item, entry, true, DEFAULT_STYLE);
   elm_entry_editable_set(entry, false);
   elm_object_part_content_set(item, "elm.swallow.content", entry);
   elm_box_pack_end(box, item);
   return entry;
}

static void
_sample_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *comp_info_box, *item, *quality;
   BOX_ADD(parent, edit->sample_box, false, false);
   elm_box_align_set(edit->sample_box, 0.5, 0.0);

   edit->snd_data.file_name = _sound_info_entry_add(edit->sample_box, _("file name"));
   edit->snd_data.type = _sound_info_entry_add(edit->sample_box, _("type"));
   // TODO: Add field with file size info.
   ITEM_ADD(edit->sample_box, item, _("compression"), "eflete/image_editor/item/default");

   BOX_ADD(edit->sample_box, comp_info_box, true, true);
   EWE_COMBOBOX_ADD(comp_info_box, edit->snd_data.comp);
   ewe_combobox_item_add(edit->snd_data.comp, "NONE");
   ewe_combobox_item_add(edit->snd_data.comp, "RAW");
   ewe_combobox_item_add(edit->snd_data.comp, "COMP");
   ewe_combobox_item_add(edit->snd_data.comp, "LOSSY");
   ewe_combobox_item_add(edit->snd_data.comp, "AS_IS");
   elm_object_disabled_set(edit->snd_data.comp, true);
   elm_box_pack_end(comp_info_box, edit->snd_data.comp);

   ITEM_ADD(item, quality, _("quality"), "eflete/image_editor/item/default");
   SPINNER_ADD(comp_info_box, edit->snd_data.quality, 45, 1000, 1, false, DEFAULT_STYLE);
   elm_object_disabled_set(edit->snd_data.quality, true);
   elm_object_part_content_set(quality, "elm.swallow.content", edit->snd_data.quality);
   elm_box_pack_end(comp_info_box, quality);
   elm_object_part_content_set(item, "elm.swallow.content", comp_info_box);
   elm_box_pack_end(edit->sample_box, item);
   evas_object_hide(edit->sample_box);
}

static void
_tone_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *item;

   BOX_ADD(parent, edit->tone_box, false, false);
   elm_box_align_set(edit->tone_box, 0.5, 0.0);

   edit->snd_data.tone_name = _sound_info_entry_add(edit->tone_box, _("name"));

   ITEM_ADD(edit->tone_box, item, "frequancy", "eflete/image_editor/item/default");
   SPINNER_ADD(edit->tone_box, edit->snd_data.tone_frq, 20, 20000, 10, false, DEFAULT_STYLE);
   elm_object_disabled_set(edit->snd_data.tone_frq, true);
   elm_object_part_content_set(item, "elm.swallow.content", edit->snd_data.tone_frq);
   elm_box_pack_end(edit->tone_box, item);
   evas_object_hide(edit->tone_box);
}

static void
_sound_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   _sample_info_create(parent, edit);
   _tone_info_create(parent, edit);
}

static void
_sample_info_setup(Sound_Editor *edit, const Item *it)
{
   Evas_Object *edje_edit_obj, *content;
   double rate;

   GET_OBJ(edit->pr, edje_edit_obj);
   content = elm_object_part_content_unset(edit->markup, "sound_info");
   evas_object_hide(content);

   elm_object_part_content_set(edit->markup, "sound_info", edit->sample_box);
   elm_entry_entry_set(edit->snd_data.file_name, it->sound_name);
   ewe_combobox_select_item_set(edit->snd_data.comp, it->comp);
   rate = edje_edit_sound_compression_rate_get(edje_edit_obj, it->sound_name);
   elm_spinner_value_set(edit->snd_data.quality, rate);
   evas_object_show(edit->sample_box);

   /* Format audio will be determined at the stage of creating the audio stream
   */
}

static void
_tone_info_setup(Sound_Editor *edit, const Item *it)
{
   Evas_Object *edje_edit_obj, *content;

   GET_OBJ(edit->pr, edje_edit_obj);
   content = elm_object_part_content_unset(edit->markup, "sound_info");
   evas_object_hide(content);

   elm_object_part_content_set(edit->markup, "sound_info", edit->tone_box);
   elm_entry_entry_set(edit->snd_data.tone_name, it->sound_name);
   elm_spinner_value_set(edit->snd_data.tone_frq, it->tone_frq);
   evas_object_show(edit->tone_box);
}

static void
_grid_sel_sample(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   const Item *item;
   Sound_Editor *edit = (Sound_Editor *)data;

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        _sample_info_setup(edit, item);
     }
}

static void
_grid_sel_tone(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   const Item *item;
   Sound_Editor *edit = (Sound_Editor *)data;

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        _tone_info_setup(edit, item);
     }
}

static Eina_Bool
_sound_content_init(Sound_Editor *edit)
{
   Eina_List *sounds = NULL, *tones, *l = NULL;
   Item *it = NULL;
   const char* sound_name = NULL;
   Evas_Object *edje_edit_obj = NULL;

   GET_OBJ(edit->pr, edje_edit_obj);

   sounds = edje_edit_sound_samples_list_get(edje_edit_obj);
   tones = edje_edit_sound_tones_list_get(edje_edit_obj);

   it = (Item *)mem_malloc(sizeof(Item));
   it->sound_name = eina_stringshare_add(_("Sound Samples"));
   elm_gengrid_item_append(edit->gengrid, ggic, it, NULL, edit);

   if (sounds)
     {
        EINA_LIST_FOREACH(sounds, l, sound_name)
          {
             it = (Item *)mem_malloc(sizeof(Item));
             it->sound_name = eina_stringshare_add(sound_name);
             it->comp = edje_edit_sound_compression_type_get(edje_edit_obj, it->sound_name);
             elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_sample, edit);
          }
        eina_list_free(sounds);
     }

   it = (Item *)mem_malloc(sizeof(Item));
   it->sound_name = eina_stringshare_add(_("Sound Tones"));
   elm_gengrid_item_append(edit->gengrid, ggic, it, NULL, edit);

   if (tones)
     {
        EINA_LIST_FOREACH(tones, l, sound_name)
          {
             it = (Item *)mem_malloc(sizeof(Item));
             it->sound_name = eina_stringshare_add(sound_name);
             it->tone_frq = edje_edit_sound_tone_frequency_get(edje_edit_obj, sound_name);
             elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_tone, edit);
          }
     }

   return true;
}

static void
_create_gengrid(Evas_Object *parent,
                Sound_Editor *editor,
                Sound_Editor_Mode mode)
{
   Evas_Object *bg;

   editor->gengrid = elm_gengrid_add(parent);
   elm_gengrid_item_size_set(editor->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_group_item_size_set(editor->gengrid, ITEM_HEIGHT/5, ITEM_HEIGHT/5);
   elm_gengrid_align_set(editor->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(editor->gengrid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   GET_IMAGE(bg, editor->gengrid, "gallery-bg");
   elm_object_part_content_set(editor->gengrid, "elm.swallow.background", bg);
   evas_object_show(bg);

   if (mode == SOUND_EDITOR_SINGLE)
     elm_gengrid_multi_select_set(editor->gengrid, false);
   else
     elm_gengrid_multi_select_set(editor->gengrid, true);

   evas_object_size_hint_weight_set(editor->gengrid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(editor->gengrid, EVAS_HINT_FILL, EVAS_HINT_FILL);

   gic = elm_gengrid_item_class_new();
   gic->item_style = DEFAULT_STYLE;
   gic->func.text_get = _grid_label_get;
   gic->func.content_get = _grid_content_get;
   gic->func.del = _grid_del;

   ggic = elm_gengrid_item_class_new();
   ggic->item_style = "group_index";
   ggic->func.text_get = _grid_label_get;
   ggic->func.content_get = NULL;
   ggic->func.del = NULL;

   if (!_sound_content_init(editor))
     {
        _sound_editor_del(editor);
        ERR("Filed initialize sound editor");
     }

   evas_object_show(editor->gengrid);
}

static inline Evas_Object *
_sound_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;
   ENTRY_ADD(parent, entry, true, "eflete/search_field");
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_ADD(entry, icon, true, "icon-search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}


/*It will be implemented*/
static void
_on_add_clicked_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{

}

static void
_on_delete_clicked_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{

}

static void
_on_srh_cnd_cb(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{

}

static void
_srh_nxt_gd_item_cb(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{

}

static void
_search_reset_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{

}

Evas_Object *
sound_editor_window_add(Project *project, Sound_Editor_Mode mode)
{
   Sound_Editor *edit;

   Evas_Object *wlayout, *btn_box, *btn, *icon, *search;

   if (!project)
     {
        ERR("Project isn't opened");
        return NULL;
     }

   edit = (Sound_Editor *)mem_calloc(1, sizeof(Sound_Editor));
   edit->pr = project;
   edit->win = mw_add(_on_quit_cb, edit);
   mw_title_set(edit->win, _("Sound editor"));

   wlayout = elm_layout_add(edit->win);
   elm_layout_file_set(wlayout, EFLETE_EDJ, "eflete/editor/default");
   elm_win_inwin_content_set(edit->win, wlayout);

   edit->markup = elm_layout_add(edit->win);
   elm_layout_file_set(edit->markup, EFLETE_EDJ, "eflete/sound_editor/default");
   evas_object_size_hint_weight_set(edit->markup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(wlayout, "eflete.swallow.content", edit->markup);
   evas_object_show(edit->markup);

   BOX_ADD(wlayout, btn_box, true, false)
   elm_box_align_set(btn_box, 1.0, 0.5);

   BUTTON_ADD(edit->win, btn, _("Apply"));
   evas_object_smart_callback_add(btn, "clicked", _on_ok_cb, edit);
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_size_hint_min_set(btn, 100, 30);
   elm_box_pack_end(btn_box, btn);
   elm_object_part_content_set(wlayout, "eflete.swallow.button_box", btn_box);
   elm_object_disabled_set(btn, true);
   evas_object_show(btn);

   BUTTON_ADD(edit->win, btn, _("Close"));
   evas_object_smart_callback_add(btn, "clicked", _on_quit_cb, edit);
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_size_hint_min_set(btn, 100, 30);
   elm_box_pack_end(btn_box, btn);
   elm_object_part_content_set(wlayout, "eflete.swallow.button_box", btn_box);
   evas_object_show(btn);

   BUTTON_ADD(edit->markup, btn, NULL);
   ICON_ADD(btn, icon, true, "icon-add");
   elm_object_content_set(btn, icon);
   evas_object_smart_callback_add(btn, "clicked", _on_add_clicked_cb, edit);
   elm_object_part_content_set(edit->markup, "swallow.add_btn", btn);

   BUTTON_ADD(edit->markup, btn, NULL);
   ICON_ADD(btn, icon, true, "icon-remove");
   elm_object_content_set(btn, icon);
   evas_object_smart_callback_add(btn, "clicked", _on_delete_clicked_cb, edit);
   elm_object_part_content_set(edit->markup, "swallow.del_btn", btn);

   search = _sound_editor_search_field_create(edit->markup);
   elm_object_part_content_set(edit->markup, "swallow.search_area", search);
   evas_object_smart_callback_add(search, "changed", _on_srh_cnd_cb, edit);
   evas_object_smart_callback_add(search, "activated", _srh_nxt_gd_item_cb, edit);
   evas_object_smart_callback_add(edit->gengrid, "pressed", _search_reset_cb,
                                  &(edit->sound_search_data));
   edit->sound_search_data.search_entry = search;
   edit->sound_search_data.last_item_found = NULL;

   _create_gengrid(edit->markup, edit, mode);
   elm_object_part_content_set(edit->markup, "gengrid", edit->gengrid);

   _sound_info_create(edit->markup, edit);

   evas_object_show(edit->win);
   return edit->win;
}

#undef ITEM_WIDTH
#undef ITEM_HEIGHT

