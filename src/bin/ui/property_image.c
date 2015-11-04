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
#include "new_history.h"
#include "editor.h"
#include "validator.h"

#include "signals.h"

#define IMAGE_PROP_DATA "image_prop_data"

#define IMAGE_PROP_DATA_GET() \
   assert(property != NULL); \
   Image_Prop_Data *pd = evas_object_data_get(property, IMAGE_PROP_DATA); \
   assert(pd != NULL);

/*
 * Callback is added for frames at property box to correct scroller
 * work while each frame would be expanded/collapsed
 */
#define FRAME_PROPERTY_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

struct _Image_Prop_Data
{
   Evas_Object *box;
   Evas_Object *image_preview;

   Evas_Object *name;
   Evas_Object *location;
   Evas_Object *type;
   Evas_Object *compression_type;
   Evas_Object *compression_quality;
   Evas_Object *size_width;
   Evas_Object *size_height;
   Evas_Object *usage_list;

   Eina_Stringshare *selected_image;
};
typedef struct _Image_Prop_Data Image_Prop_Data;

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
_on_image_selected(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
}

Evas_Object *
ui_property_image_add(Evas_Object *parent)
{
   Image_Prop_Data *pd;
   Evas_Object *item;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Image_Prop_Data));

   /* editors */
   BOX_ADD(parent, pd->box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);

   pd->image_preview = elm_layout_add(parent);
   elm_layout_theme_set(pd->image_preview, "layout", "image_editor", "preview");
   elm_box_pack_end(pd->box, pd->image_preview);
   evas_object_size_hint_weight_set(pd->image_preview, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->image_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(pd->image_preview);

   evas_object_data_set(pd->box, IMAGE_PROP_DATA, pd);

   item = prop_item_label_add(pd->box, &pd->name, _("name"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_item_label_add(pd->box, &pd->location, _("location"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_item_label_add(pd->box, &pd->type, _("type"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_image_editor_compression_type_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);
   item = prop_image_editor_compression_quality_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);

   item = prop_item_label_add(pd->box, &pd->size_width, _("image width"), _(" - "));
   elm_box_pack_end(pd->box, item);

   item = prop_item_label_add(pd->box, &pd->size_height, _("image height"), _(" - "));
   elm_box_pack_end(pd->box, item);

/*********************************/
/*   ENTRY_ADD(layout, img_edit->image_data_fields.file_name, true);
   elm_entry_editable_set(img_edit->image_data_fields.file_name, false);
   elm_object_part_content_set(layout, "eflete.swallow.image_info.name",
                               img_edit->image_data_fields.file_name);

   EWE_COMBOBOX_ADD(layout, img_edit->image_data_fields.comp);
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "NONE");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "RAW");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "USER");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "COMP");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "LOSSY");
   elm_object_disabled_set(img_edit->image_data_fields.comp, true);
   elm_object_part_content_set(layout, "eflete.swallow.compression_type",
                               img_edit->image_data_fields.comp);

   SPINNER_ADD(layout, img_edit->image_data_fields.quality, 0, 100, 1, false);
   elm_object_disabled_set(img_edit->image_data_fields.quality, true);
   elm_object_part_content_set(layout, "eflete.swallow.compression_quality",
                               img_edit->image_data_fields.quality); */
/********************************/

   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_SELECTED, _on_image_selected, pd->box);

   return pd->box;
}
