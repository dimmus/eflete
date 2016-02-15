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
#include "modal_window.h"
#include "config.h"

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define SND_EDIT_KEY "sound_editor_key"

typedef struct _Sound_Editor Sound_Editor;
typedef struct _Search_Data Search_Data;
typedef struct _Sound Sound;

struct _Sound
{
   const char *name;
   const char *src;
   int tone_frq;
};

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Sound_Editor
{
   Evas_Object *popup;
   Evas_Object *popup_btn_add;
   Evas_Object *btn_add;
   Evas_Object *btn_del;
   Evas_Object *menu;
   Evas_Object *tone_entry, *frq_entry;
   Elm_Validator_Regexp *tone_validator, *frq_validator;
   Evas_Object *gengrid;
   Evas_Object *markup;
   Elm_Object_Item *tone_header;
   Search_Data sound_search_data;
   Evas_Object *check;
   const char  *snd_src;
};

static Elm_Gengrid_Item_Class *gic = NULL, *ggic = NULL;

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part)
{
   Sound_Data *snd = (Sound_Data *)data;

   if (!part) goto exit;

   if (!strcmp(part, "elm.text"))
     return strdup(snd->name);

   if (!strcmp(part, "elm.text.type"))
     return strdup(snd->type_label);

exit:
   return NULL;
}

static void
_grid_del(void *data, Evas_Object *obj __UNUSED__)
{
   Sound_Data *snd = (Sound_Data *)data;

   eina_stringshare_del(snd->name);
   eina_stringshare_del(snd->type_label);
   free(snd);
}

static Eina_Stringshare *
_sound_format_get(Eina_Stringshare *snd_src)
{
   Eina_Stringshare *type;
   char *dot, *up;

   assert(snd_src != NULL);

   dot = strrchr(snd_src, '.');
   if (!dot)
     return eina_stringshare_add(_("UNKNOWN"));

   dot++;
   up = strdup(dot);
   eina_str_toupper(&up);
   type = eina_stringshare_add((const char *)up);
   free(up);
   return type;
}

static void
_grid_selected(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Sound_Editor *edit = (Sound_Editor *)data;
   External_Resource *res = NULL;
   Sound_Data *snd;

   assert(edit != NULL);

   snd = elm_object_item_data_get((Elm_Gengrid_Item *)event_info);
   switch (snd->type)
     {
      case SOUND_TYPE_SAMPLE:
         res = pm_resource_get(ap.project->sounds, snd->name);
         break;
      case SOUND_TYPE_TONE:
         res = pm_resource_get(ap.project->tones, snd->name);
         break;
     }

   assert(res != NULL);

   if (!res->used_in) elm_object_disabled_set(edit->btn_del, false);
   evas_object_smart_callback_call(ap.win, SIGNAL_SOUND_SELECT, snd);
}

static void
_gengrid_content_fill(Sound_Editor *edit)
{
   Eina_List *l;
   External_Resource *sample;
   Tone_Resource *tone;
   Sound_Data *snd;

   assert(edit != NULL);
   assert(ap.project != NULL);

   snd = (Sound_Data *)mem_calloc(1, sizeof(Sound_Data));
   snd->name = eina_stringshare_add(_("Sound Samples"));
   elm_gengrid_item_append(edit->gengrid, ggic, snd, NULL, NULL);
   EINA_LIST_FOREACH(ap.project->sounds, l, sample)
     {
        snd = (Sound_Data *)mem_calloc(1, sizeof(Sound_Data));
        snd->name = eina_stringshare_ref(sample->name);
        snd->type_label = _sound_format_get(sample->source);
        snd->type = SOUND_TYPE_SAMPLE;
        snd->resource = (Resource *)sample;
        elm_gengrid_item_append(edit->gengrid, gic, snd, _grid_selected, edit);
     }

   snd = (Sound_Data *)mem_calloc(1, sizeof(Sound_Data));
   snd->name = eina_stringshare_add(_("Sound Tones"));
   edit->tone_header = elm_gengrid_item_append(edit->gengrid, ggic, snd, NULL, NULL);
   EINA_LIST_FOREACH(ap.project->tones, l, tone)
     {
        snd = (Sound_Data *)mem_calloc(1, sizeof(Sound_Data));
        snd->name = eina_stringshare_ref(tone->name);
        snd->type_label = eina_stringshare_printf("%d", tone->freq);
        snd->type = SOUND_TYPE_TONE;
        snd->resource = (Resource *)tone;
        elm_gengrid_item_append(edit->gengrid, gic, snd, _grid_selected, edit);
     }
}

static void
_grid_sel(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Sound_Editor *editor = (Sound_Editor *)data;

   elm_object_disabled_set(editor->btn_del, true);
   evas_object_smart_callback_call(ap.win, SIGNAL_SOUND_UNSELECTED, NULL);
}

static void
_create_gengrid(Evas_Object *parent, Sound_Editor *editor)
{
   assert(parent != NULL);
   assert(editor != NULL);

   editor->gengrid = elm_gengrid_add(parent);
   elm_gengrid_item_size_set(editor->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_group_item_size_set(editor->gengrid, ITEM_HEIGHT/3, ITEM_HEIGHT/3);
   elm_gengrid_align_set(editor->gengrid, 0.0, 0.0);
   evas_object_smart_callback_add(editor->gengrid, "unselected", _grid_sel, editor);
   elm_scroller_policy_set(editor->gengrid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   elm_gengrid_multi_select_set(editor->gengrid, false);

   evas_object_size_hint_weight_set(editor->gengrid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(editor->gengrid, EVAS_HINT_FILL, EVAS_HINT_FILL);

   gic = elm_gengrid_item_class_new();
   gic->item_style = "sound_editor";
   gic->func.text_get = _grid_label_get;
   gic->func.content_get = NULL;
   gic->func.del = _grid_del;

   ggic = elm_gengrid_item_class_new();
   ggic->item_style = "group_index";
   ggic->func.text_get = _grid_label_get;
   ggic->func.content_get = NULL;
   ggic->func.del = _grid_del;

   _gengrid_content_fill(editor);

   evas_object_show(editor->gengrid);
   elm_object_part_content_set(editor->markup, "eflete.swallow.grid", editor->gengrid);
}

static inline Evas_Object *
_sound_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static Eina_Bool
_add_sample_done(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Sound_Data *snd;
   Eina_Stringshare *sound_name;
   Eina_List *samples_list, *l;
   Eina_Bool exist = false;
   External_Resource *res;
   const char *file_name;

   Eina_List *selected_list = (Eina_List *)event_info;
   const char *selected = eina_list_data_get(selected_list);
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   samples_list = ap.project->sounds;

   if (!ecore_file_exists(selected))
     {
        ERR(_("File '%s' is not exist"), selected);
        return false;
     }
   if (ecore_file_is_dir(selected))
     {
        ERR(_("Unable to add folder"))
        return true;
     }

   file_name = ecore_file_file_get(selected);
   sound_name = eina_stringshare_add(file_name);
   EINA_LIST_FOREACH(samples_list, l, res)
      if (res->name == sound_name) /* they both are stringshares */
        {
           exist = true;
           break;
        }
   if (exist)
     {
        WARN(_("Sample '%s' is already added to project"), sound_name)
        eina_stringshare_del(sound_name);
        return false;
     }

   res = mem_calloc(1, sizeof(External_Resource));
   res->name = eina_stringshare_add(file_name);
   res->source = eina_stringshare_printf("%s/sounds/%s", ap.project->develop_path, file_name);

   if (!ecore_file_exists(res->source))
     {
        ecore_file_cp(selected, res->source);

        ap.project->sounds = eina_list_sorted_insert(ap.project->sounds, (Eina_Compare_Cb) resource_cmp, res);
     }
   else
     {
        ERR(_("File '%s' exist"), res->name);
        free(res);
        return true;
     }

   edje_edit_sound_sample_add(ap.project->global_object, res->name, res->source);

   snd = (Sound_Data *)mem_malloc(sizeof(Sound_Data));
   snd->name = eina_stringshare_ref(res->name);
   snd->type_label = _sound_format_get(res->source);
   snd->type = SOUND_TYPE_SAMPLE;
   snd->resource = (Resource *)res;
   elm_gengrid_item_insert_before(edit->gengrid, gic, snd, edit->tone_header, _grid_selected, edit);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_sound_sample_add would be added into Editor Module and saving would work properly")
   ap.project->changed = true;

   return true;
}

static void
_popup_close(void *data)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);
   assert(edit->tone_validator != NULL);
   assert(edit->frq_validator != NULL);

   evas_object_del(edit->popup);

   elm_validator_regexp_free(edit->tone_validator);
   elm_validator_regexp_free(edit->frq_validator);
   edit->tone_validator = NULL;
   edit->frq_validator = NULL;

   elm_gengrid_item_selected_set(elm_gengrid_last_item_get(edit->gengrid), true);
}

static void
_add_tone_cancel(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   ecore_job_add(_popup_close, edit);
}

static void
_add_tone_done(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Sound_Data *snd;
   Eina_Stringshare *tone_name;
   int frq;
   Eina_List *tones_list, *l;
   Eina_Bool exist = false;
   Tone_Resource *tone;

   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   tone_name = eina_stringshare_add(elm_entry_entry_get(edit->tone_entry));

   tones_list = ap.project->tones;
   EINA_LIST_FOREACH(tones_list, l, tone)
     if (tone->name == tone_name) /* they both are stringshares */
       {
          exist = true;
          break;
       }
   if (exist)
     {
        WARN(_("Tone '%s' is already added to project"), tone_name)
        eina_stringshare_del(tone_name);
        return;
     }

   frq = atoi(elm_entry_entry_get(edit->frq_entry));
   edje_edit_sound_tone_add(ap.project->global_object, tone_name, frq);

   tone = (Tone_Resource *)mem_calloc(1, sizeof(Tone_Resource));
   tone->name = eina_stringshare_add(tone_name);
   tone->freq = frq;
   ap.project->tones = eina_list_sorted_insert(ap.project->tones, (Eina_Compare_Cb)resource_cmp, tone);

   snd = (Sound_Data *)mem_malloc(sizeof(Sound_Data));
   snd->name = eina_stringshare_ref(tone->name);
   snd->type_label = eina_stringshare_add(elm_entry_entry_get(edit->frq_entry));
   snd->type = SOUND_TYPE_TONE;
   snd->resource = (Resource *)tone;
   elm_gengrid_item_append(edit->gengrid, gic, snd, _grid_selected, edit);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_image_add would be added into Editor Module and saving would work properly")
   ap.project->changed = true;

   ecore_job_add(_popup_close, edit);
}

static void
_sample_add_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   popup_fileselector_sound_helper(_("Choose a sound"), NULL, NULL, _add_sample_done, data, false, false);
}

static void
_validation(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   if ((elm_validator_regexp_status_get(edit->tone_validator) != ELM_REG_NOERROR) ||
       (elm_validator_regexp_status_get(edit->frq_validator) != ELM_REG_NOERROR))
     elm_object_disabled_set(edit->popup_btn_add, true);
   else
     elm_object_disabled_set(edit->popup_btn_add, false);
}

static void
_tone_add_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;

   assert(edit != NULL);

   Evas_Object *popup, *box, *item, *bt_no;
   Eina_Stringshare *title;

   popup = elm_popup_add(ap.win);
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);
   title = eina_stringshare_add(_("Add new tone to the project"));
   elm_object_part_text_set(popup, "title,text", title);
   edit->popup = popup;

   assert(edit->tone_validator == NULL);
   assert(edit->frq_validator == NULL);
   edit->tone_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   edit->frq_validator = elm_validator_regexp_new(FREQUENCY_REGEX, NULL);

   BOX_ADD(popup, box, false, false);
   elm_object_content_set(popup, box);
   LAYOUT_PROP_ADD(box, _("Tone name:"), "property", "1swallow")
   ENTRY_ADD(item, edit->tone_entry, true);
   eo_do(edit->tone_entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, edit->tone_validator));
   evas_object_smart_callback_add(edit->tone_entry, "changed", _validation, edit);
   elm_object_part_text_set(edit->tone_entry, "guide", _("Type a new tone name"));
   /* need to manualy set not valid string for triggered validator */
   elm_entry_entry_set(edit->tone_entry, NULL);
   elm_object_part_content_set(item, "elm.swallow.content", edit->tone_entry);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Frequency:"), "property", "1swallow")
   ENTRY_ADD(item, edit->frq_entry, true);
   eo_do(edit->frq_entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, edit->frq_validator));
   evas_object_smart_callback_add(edit->frq_entry, "changed", _validation, edit);
   elm_object_part_text_set(edit->frq_entry, "guide", _("Type a frequency (20 - 20000)"));
   /* need to manualy set not valid string for triggered validator */
   elm_entry_entry_set(edit->frq_entry, NULL);
   elm_object_part_content_set(item, "elm.swallow.content", edit->frq_entry);
   elm_box_pack_end(box, item);

   BUTTON_ADD(popup, edit->popup_btn_add, _("Add"));
   evas_object_smart_callback_add (edit->popup_btn_add, "clicked", _add_tone_done, edit);
   elm_object_part_content_set(popup, "button1", edit->popup_btn_add);
   elm_object_disabled_set(edit->popup_btn_add, true);

   BUTTON_ADD(popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _add_tone_cancel, edit);
   elm_object_part_content_set(popup, "button2", bt_no);

   evas_object_show(popup);
   eina_stringshare_del(title);
}

#undef INFO_ADD

static void
_on_btn_plus_clicked(void *data,
                     Evas_Object *obj,
                     void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;
   Evas_Coord x, y, h;

   assert(edit != NULL);

   evas_object_geometry_get(obj, &x, &y, NULL, &h);

   elm_menu_move(edit->menu, x, y + h);
   evas_object_show(edit->menu);
}

static void
_on_delete_clicked_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Elm_Object_Item *grid_it;
   Sound_Data *snd;
   Eina_List *list, *l, *l_next;
   Sound_Editor *edit = (Sound_Editor *)data;
   External_Resource *res;

   assert(edit != NULL);

   list = (Eina_List *)elm_gengrid_selected_items_get(edit->gengrid);
   EINA_LIST_FOREACH_SAFE(list, l, l_next, grid_it)
     {
        snd = elm_object_item_data_get(grid_it);

        switch (snd->type)
          {
           case SOUND_TYPE_SAMPLE:
              res = pm_resource_get(ap.project->sounds, snd->name);
              if (res->used_in) ERR("Unable to delete sample '%s'", res->name);
              edje_edit_sound_sample_del(ap.project->global_object, snd->name);
              ecore_file_unlink(res->source);
              ap.project->sounds = pm_resource_del(ap.project->sounds, res);
              elm_object_item_del(grid_it);
              break;
           case SOUND_TYPE_TONE:
              res = pm_resource_get(ap.project->tones, snd->name);
              if (res->used_in) ERR("Unable to delete tone '%s'", res->name);
              edje_edit_sound_tone_del(ap.project->global_object, snd->name);
              ap.project->tones = pm_resource_del(ap.project->tones, res);
              elm_object_item_del(grid_it);
              break;
          }
     }

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_sound_..._del would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;

   elm_object_disabled_set(edit->btn_del, true);
   evas_object_smart_callback_call(ap.win, SIGNAL_SOUND_UNSELECTED, NULL);
}

ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, "elm.label")

static void
_search_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;

   assert(edit != NULL);

   _gengrid_item_search(edit->gengrid, &(edit->sound_search_data),
                        edit->sound_search_data.last_item_found);

}

static void
_search_nxt_gd_item(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;
   Elm_Object_Item *start_from = NULL;

   assert(edit != NULL);

   if (edit->sound_search_data.last_item_found)
     {
        start_from =
           elm_gengrid_item_next_get(edit->sound_search_data.last_item_found);
     }

   _gengrid_item_search(edit->gengrid, &(edit->sound_search_data), start_from);
}

static void
_search_reset_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Search_Data *search_data = data;

   assert(search_data != NULL);

   search_data->last_item_found = NULL;
}

static void
_sound_editor_main_markup_create(Sound_Editor *edit)
{
   Evas_Object *ic, *search;

   assert(edit != NULL);

   edit->markup = elm_layout_add(ap.win);
   elm_layout_theme_set(edit->markup, "layout", "sound_manager", "default");
   evas_object_size_hint_weight_set(edit->markup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_data_set(edit->markup, SND_EDIT_KEY, edit);

   edit->btn_del = elm_button_add(edit->markup);
   elm_object_style_set(edit->btn_del, "anchor");
   evas_object_smart_callback_add(edit->btn_del, "clicked", _on_delete_clicked_cb, edit);
   elm_object_part_content_set(edit->markup, "eflete.swallow.del_btn", edit->btn_del);

   ic = elm_icon_add(edit->btn_del);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(edit->btn_del, NULL, ic);
   elm_object_disabled_set(edit->btn_del, true);

   edit->btn_add = elm_button_add(edit->markup);
   elm_object_style_set(edit->btn_add, "anchor");
   evas_object_smart_callback_add(edit->btn_add, "clicked", _on_btn_plus_clicked, edit);
   elm_object_part_content_set(edit->markup, "eflete.swallow.add_btn", edit->btn_add);

   ic = elm_icon_add(edit->btn_add);
   elm_icon_standard_set(ic, "plus");
   elm_object_part_content_set(edit->btn_add, NULL, ic);

   edit->menu = elm_menu_add(ap.win);
   elm_menu_item_add(edit->menu, NULL, NULL, _("Sample"), _sample_add_cb, edit);
   elm_menu_item_add(edit->menu, NULL, NULL, _("Tone"), _tone_add_cb, edit);

   search = _sound_editor_search_field_create(edit->markup);
   evas_object_hide(search);
   elm_layout_content_set(edit->markup, "eflete.swallow.search_area", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, edit);
   evas_object_smart_callback_add(edit->gengrid, "pressed", _search_reset_cb,
                                  &(edit->sound_search_data));
   edit->sound_search_data.search_entry = search;
   edit->sound_search_data.last_item_found = NULL;
}

Evas_Object *
sound_manager_add(void)
{
   Sound_Editor *edit;

   assert(ap.project != NULL);

#ifdef HAVE_AUDIO
   ecore_audio_init();
#endif

   edit = (Sound_Editor *)mem_calloc(1, sizeof(Sound_Editor));

   _sound_editor_main_markup_create(edit);
   _create_gengrid(edit->markup, edit);
   elm_object_focus_set(edit->sound_search_data.search_entry, true);

   evas_object_smart_callback_call(ap.win, SIGNAL_SOUND_UNSELECTED, NULL);
   return edit->markup;
}
