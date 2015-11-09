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

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "string_common.h"
#include "common_macro.h"
#include "new_history.h"
#include "editor.h"
#include "validator.h"

#include "signals.h"

#define IMAGE_PROP_DATA "image_prop_data"

#define IMAGE_PROP_DATA_GET() \
   assert(property != NULL); \
   Image_Prop_Data *pd = evas_object_data_get(property, IMAGE_PROP_DATA); \
   assert(pd != NULL);

typedef struct _Search_Data Search_Data;
struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};
struct _Image_Prop_Data
{
   Evas_Object *box;
   Evas_Object *preview_image;
   Evas_Object *preview_frame;

   Evas_Object *name;
   Evas_Object *location;
   Evas_Object *type;
   Evas_Object *compression_type;
   Evas_Object *compression_quality;
   Evas_Object *size_width;
   Evas_Object *size_height;
   Evas_Object *usage_list;
   Evas_Object *info_frame;

   Search_Data usage_search_data;
   Evas_Object *usage_genlist;
   Evas_Object *usage_frame;

   Eina_Stringshare *selected_image;
};
typedef struct _Image_Prop_Data Image_Prop_Data;

static Elm_Genlist_Item_Class *_itc_group = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;
static Elm_Genlist_Item_Class *_itc_state = NULL;

/* accroding to Edje_Edit.h */
static const char *edje_image_compression[] = { N_("RAW"),
                                                N_("USER"),
                                                N_("COMP"),
                                                N_("LOSSY"),
                                                N_("LOSSY_ETC1"),
                                                N_("LOSSY_ETC2"),
                                                NULL};

static Evas_Object *
prop_image_editor_compression_type_add(Evas_Object *property, Image_Prop_Data *pd);

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   assert(parent != NULL);
   assert(label != NULL);

   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

/* TODO this is not implemented yet, please remake it!!! */
static Evas_Object *
prop_image_editor_compression_quality_add(Evas_Object *parent,
                                          Image_Prop_Data *pd)
{
   PROPERTY_ITEM_ADD(parent, _("compression quality/rate"), "2swallow")
   SPINNER_ADD(item, pd->compression_quality, 0, 100, 1, false)
   elm_layout_content_set(item, "swallow.content1", pd->compression_quality);
   elm_object_disabled_set(pd->compression_quality, true);
   elm_spinner_value_set(pd->compression_quality, 0);
   return item;
}
static void
_on_image_compression_type_change(void *data,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info)
{
   Image_Prop_Data *pd = (Image_Prop_Data *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;
   if (!edje_edit_image_compression_type_set(ap.project->global_object, pd->selected_image, (int)item->index))
     {
        ERR("Cann't apply value '%s' for image.", item->title);
        abort();
     }
}
static Evas_Object *
prop_image_editor_compression_type_add(Evas_Object *parent, Image_Prop_Data *pd)
{
   int i;
   PROPERTY_ITEM_ADD(parent, _("image compression type"), "1swallow");
   EWE_COMBOBOX_ADD(parent, pd->compression_type);
   /* disable for now */
   elm_object_disabled_set(pd->compression_type, true);
   for (i = 0; edje_image_compression[i]; ewe_combobox_item_add(pd->compression_type, edje_image_compression[i]), i++);

   evas_object_smart_callback_add(pd->compression_type, "selected", _on_image_compression_type_change, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->compression_type);

   return item;
}

static void
_grid_group_item_del(void *data, Evas_Object *obj __UNUSED__)
{
   if (data)
     eina_stringshare_del(data);
}
static char *
_grid_group_item_label_get(void *data,
                           Evas_Object *obj __UNUSED__,
                           const char  *part)
{
   if ((part) && (!strcmp(part, "elm.text")))
     return strdup(data);
   else
     return NULL;
}
static inline void
genlist_item_classes_init()
{
   if (!_itc_group)
     {
        _itc_group = elm_genlist_item_class_new();
        _itc_group->item_style = "title";
        _itc_group->func.text_get = _grid_group_item_label_get;
        _itc_group->func.content_get = NULL;
        _itc_group->func.state_get = NULL;
        _itc_group->func.del = _grid_group_item_del;
     }
   if (!_itc_part)
     {
        _itc_part = elm_genlist_item_class_new();
        _itc_part->item_style = "default";
        _itc_part->func.text_get = _grid_group_item_label_get;
        _itc_part->func.content_get = NULL;
        _itc_part->func.state_get = NULL;
        _itc_part->func.del = _grid_group_item_del;
     }
   if (!_itc_state)
     {
        _itc_state = elm_genlist_item_class_new();
        _itc_state->item_style = "default";
        _itc_state->func.text_get = _grid_group_item_label_get;
        _itc_state->func.content_get = NULL;
        _itc_state->func.state_get = NULL;
        _itc_state->func.del = _grid_group_item_del;
     }
}
static void
_image_info_type_setup(Evas_Object *layout,
                       const char *image_name)
{
   char buf[BUFF_MAX];

   assert(layout != NULL);
   assert(image_name != NULL);

   const char *dot = strrchr(image_name, '.');
   if ((!dot) || (dot == image_name))
     {
        elm_object_text_set(layout, _("Unknown"));
        return;
     }

   if (eina_str_has_extension(image_name, ".png"))
     snprintf(buf, BUFF_MAX, _("PNG image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".gif"))
     snprintf(buf, BUFF_MAX, _("GIF image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".jpeg") ||
            eina_str_has_extension(image_name, ".jpg") ||
            eina_str_has_extension(image_name, ".jif") ||
            eina_str_has_extension(image_name, ".jfif"))
     snprintf(buf, BUFF_MAX, _("JPEG image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".bmp"))
     snprintf(buf, BUFF_MAX, _("BMP image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".tif") ||
            eina_str_has_extension(image_name, ".tiff"))
     snprintf(buf, BUFF_MAX, _("TIFF image (%s)"), dot);
   else
     {
        elm_object_text_set(layout, _("Unknown"));
        return;
     }
   elm_object_text_set(layout, buf);
}
static inline void
_image_info_update_usage_info(Image_Prop_Data *pd, int list_count)
{
   Eina_Stringshare *title;

   assert(pd != NULL);

   if (list_count)
     title = eina_stringshare_printf(_("Usage (%d)"), list_count);
   else
     title = eina_stringshare_add(_("Usage ( - )"));

   elm_object_disabled_set(pd->usage_search_data.search_entry,
                           !list_count);
   elm_entry_entry_set(pd->usage_search_data.search_entry, "");
   pd->usage_search_data.last_item_found = NULL;

   elm_object_text_set(pd->usage_frame, title);
   eina_stringshare_del(title);
}
static void
_image_info_usage_update(Image_Prop_Data *pd, const char *image_name)
{
   Eina_List *l;
   Evas_Object *list;
   Elm_Object_Item *it_group, *it_part;
   const char *cur_group = NULL;
   const char *cur_part = NULL;
   State *state;

   assert(pd != NULL);
   assert(image_name != NULL);

   list = pd->usage_genlist;
   elm_genlist_clear(list);
   if (!pd->selected_image) return;

   Resource *res = (Resource *) pm_resource_get(ap.project->images, image_name);
   _image_info_update_usage_info(pd, eina_list_count(res->used_in));
   if (!eina_list_count(res->used_in))
     return;

   genlist_item_classes_init();

   EINA_LIST_FOREACH(res->used_in, l, state)
     {
        if ((!cur_group) || (strcmp(cur_group, state->part->group->name)))
          {
             it_group = elm_genlist_item_append(list, _itc_group,
                                                eina_stringshare_add(state->part->group->name),
                                                NULL, ELM_GENLIST_ITEM_NONE,
                                                NULL, NULL);
             cur_group = state->part->group->name;
             cur_part = NULL;
          }
        if ((!cur_part) || (strcmp(cur_part, state->part->name)))
          {
             it_part = elm_genlist_item_append(list, _itc_part,
                                               eina_stringshare_add(state->part->name),
                                               it_group, ELM_GENLIST_ITEM_NONE,
                                               NULL, NULL);
             cur_part = state->part->name;
          }
        if ((cur_part) && (!strcmp(cur_part, state->part->name)))
          {
             elm_genlist_item_append(list, _itc_state, eina_stringshare_add(state->name),
                                     it_part, ELM_GENLIST_ITEM_NONE,
                                     NULL, NULL);
          }
     }
}
static void
_on_image_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *image = (Evas_Object *)event_info;
   Eina_Stringshare *image_name;
   Edje_Edit_Image_Comp comp;
   Eina_Stringshare *str;
   int w, h;

   Evas_Object *property = data;
   IMAGE_PROP_DATA_GET()

   if (image)
     {
        elm_object_part_content_set(pd->preview_image, "eflete.swallow.image", image);

        image_name = evas_object_data_get(image, "image_name");
        pd->selected_image = image_name;

        comp =  edje_edit_image_compression_type_get(ap.project->global_object, image_name);
        if (comp != EDJE_EDIT_IMAGE_COMP_USER)
          {
             str = eina_stringshare_printf("edje/images/%i", edje_edit_image_id_get(ap.project->global_object, image_name));
             elm_object_text_set(pd->location, str);
             eina_stringshare_del(str);
          }
        else
          elm_object_text_set(pd->location, image_name);

        if (comp == EDJE_EDIT_IMAGE_COMP_LOSSY)
          {
             int quality = edje_edit_image_compression_rate_get(ap.project->global_object,
                                                                image_name);
             elm_spinner_value_set(pd->compression_quality, quality);
          }
        else
          elm_spinner_value_set(pd->compression_quality, 0);

        elm_image_object_size_get(image, &w, &h);
        str = eina_stringshare_printf("%d", w);
        elm_object_text_set(pd->size_width, str);
        eina_stringshare_del(str);
        str = eina_stringshare_printf("%d", h);
        elm_object_text_set(pd->size_height, str);
        eina_stringshare_del(str);

        _image_info_type_setup(pd->type, image_name);
        _image_info_usage_update(pd, image_name);
     }
   else
     {
        image = elm_object_part_content_unset(pd->preview_image, "eflete.swallow.image");
        evas_object_del(image);
        elm_object_text_set(pd->name, _(" - "));
        elm_object_text_set(pd->location, _(" - "));
        elm_object_text_set(pd->size_width, _(" - "));
        elm_object_text_set(pd->size_height, _(" - "));
        elm_object_text_set(pd->type, _(" - "));
        ewe_combobox_select_item_set(pd->compression_type, 0);
        elm_spinner_value_set(pd->compression_quality, 0);
        pd->selected_image = NULL;
     }
}

ITEM_SEARCH_FUNC(genlist, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE, NULL)
static void
_on_usage_search_entry_changed_cb(void *data,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info __UNUSED__)
{
   Image_Prop_Data *pd = data;

   assert(pd != NULL);

   _genlist_item_search(pd->usage_genlist,
                        &(pd->usage_search_data),
                        pd->usage_search_data.last_item_found);
}
static void
_search_next_genlist_item_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Prop_Data *pd = data;
   Elm_Object_Item *start_from = NULL;

   assert(pd != NULL);

   if (pd->usage_search_data.last_item_found)
     {
        start_from =
           elm_genlist_item_next_get(pd->usage_search_data.last_item_found);
     }
   _genlist_item_search(pd->usage_genlist,
                        &(pd->usage_search_data), start_from);
}
static inline Evas_Object *
_image_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}
static Evas_Object *
_image_usage_layout_create(Image_Prop_Data *pd)
{
   Evas_Object *layout = NULL;
   Evas_Object *genlist = NULL;
   Evas_Object *entry = NULL;
   Evas_Object *parent = NULL;

   assert(pd != NULL);
   parent = pd->usage_frame;

   layout = elm_layout_add(parent);
   elm_layout_theme_set(layout, "layout", "image_editor", "usage_info");
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(layout);

   genlist = elm_genlist_add(layout);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(genlist);
   elm_object_part_content_set(layout, "eflete.swallow.genlist", genlist);
   pd->usage_genlist = genlist;

   entry = _image_editor_search_field_create(layout);
   elm_object_part_content_set(layout, "eflete.swallow.search_line", entry);
   evas_object_smart_callback_add(entry, "changed",
                                  _on_usage_search_entry_changed_cb, pd);
   evas_object_smart_callback_add(entry, "activated",
                                  _search_next_genlist_item_cb, pd);
   pd->usage_search_data.search_entry = entry;
   pd->usage_search_data.last_item_found = NULL;
   evas_object_hide(layout);
   return layout;
}

Evas_Object *
ui_property_image_add(Evas_Object *parent)
{
   Image_Prop_Data *pd;
   Evas_Object *item, *box, *layout;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Image_Prop_Data));

   /* editors */
   BOX_ADD(parent, pd->box, false, false);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);
   evas_object_data_set(pd->box, IMAGE_PROP_DATA, pd);

   /* Frame with preview */
   FRAME_PROPERTY_ADD(pd->box, pd->preview_frame, true, _("Preview"), pd->box)
   pd->preview_image = elm_layout_add(parent);
   elm_layout_theme_set(pd->preview_image, "layout", "image_editor", "preview");
   evas_object_size_hint_weight_set(pd->preview_image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->preview_image, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(pd->preview_image);
   elm_object_content_set(pd->preview_frame, pd->preview_image);
   elm_box_pack_end(pd->box, pd->preview_frame);

   /* Frame with info */
   FRAME_PROPERTY_ADD(pd->box, pd->info_frame, true, _("Info"), pd->box)
   BOX_ADD(pd->info_frame, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   item = prop_item_label_add(box, &pd->name, _("name"), _(" - "));
   elm_box_pack_end(box, item);
   item = prop_item_label_add(box, &pd->location, _("location"), _(" - "));
   elm_box_pack_end(box, item);
   item = prop_item_label_add(box, &pd->type, _("type"), _(" - "));
   elm_box_pack_end(box, item);
   item = prop_image_editor_compression_type_add(box, pd);
   elm_box_pack_end(box, item);
   item = prop_image_editor_compression_quality_add(box, pd);
   elm_box_pack_end(box, item);
   item = prop_item_label_add(box, &pd->size_width, _("image width"), _(" - "));
   elm_box_pack_end(box, item);
   item = prop_item_label_add(box, &pd->size_height, _("image height"), _(" - "));
   elm_box_pack_end(box, item);
   elm_object_content_set(pd->info_frame, box);
   elm_box_pack_end(pd->box, pd->info_frame);

   /* Frame with usage */
   FRAME_PROPERTY_ADD(pd->box, pd->usage_frame, true, _("Usage ( - )"), pd->box)
   BOX_ADD(pd->info_frame, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);

   layout = _image_usage_layout_create(pd);

   elm_object_content_set(pd->usage_frame, layout);
   elm_box_pack_end(pd->box, pd->usage_frame);

   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_SELECTED, _on_image_selected, pd->box);

   return pd->box;
}
