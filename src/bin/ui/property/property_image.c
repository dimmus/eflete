/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "main_window.h"
#include "project_manager.h"
#include "property.h"
#include "property_private.h"

#define EMPTY_VALUE " - "

typedef struct {
   Property_Attribute item_preview;
   Property_Attribute item_name;
   Property_Attribute item_location;
   Property_Attribute item_type;
   Property_Attribute item_image_compression;
   Property_Attribute item_image_quality;
   Property_Attribute item_width;
   Property_Attribute item_height;

   Image_Item *image;
} Property_Image_Data;

/* accroding to Edje_Edit.h */
static const char *edje_image_compression[] = { "RAW",
                                                "USER",
                                                "COMP",
                                                "LOSSY",
                                                "LOSSY_ETC1",
                                                "LOSSY_ETC2",
                                                NULL};

static Property_Image_Data image_data;

static void
_image_info_type_setup(Property_Action *action, const char *image_name)
{
   char buf[BUFF_MAX];

   assert(image_name != NULL);

   const char *dot = strrchr(image_name, '.');
   if ((!dot) || (dot == image_name))
     {
        elm_object_text_set(action->control, _("Unknown"));
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
        elm_object_text_set(action->control, _("Unknown"));
        return;
     }
   elm_object_text_set(action->control, buf);
}

static void
_update_cb(Property_Attribute *pa __UNUSED__, Property_Action *action)
{
   Eina_Stringshare *str;
   int w, h;

   if (!image_data.image)
     {
        if (action->type.attribute_image != PROPERTY_IMAGE_CONTROL_PREVIEW)
          elm_object_text_set(action->control, EMPTY_VALUE);
        else
          elm_image_file_set(action->control, EFLETE_IMG_PATH EFLETE_DUMMY_IMAGE_NAME, NULL);
        return;
     }

   switch (action->type.attribute_image)
     {
      case PROPERTY_IMAGE_CONTROL_PREVIEW:
         if (image_data.image->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
           {
              if (ecore_file_exists(image_data.image->source))
                elm_image_file_set(action->control, image_data.image->source, NULL);
              else
                elm_image_file_set(action->control, EFLETE_THEME, "elm/image/icon/attention");
           }
         else
           {
              elm_image_file_set(action->control, image_data.image->source, NULL);
           }
         evas_object_image_smooth_scale_set(action->control, false);
         break;
      case PROPERTY_IMAGE_CONTROL_NAME:
         elm_object_text_set(action->control, image_data.image->image_name);
         break;
      case PROPERTY_IMAGE_CONTROL_LOCATION:
         if (image_data.image->comp_type != EDJE_EDIT_IMAGE_COMP_USER)
           {
              str = eina_stringshare_printf("edje/images/%i", image_data.image->id);
              elm_object_text_set(action->control, str);
              eina_stringshare_del(str);
           }
         else
           elm_object_text_set(action->control, image_data.image->image_name);
         break;
      case PROPERTY_IMAGE_CONTROL_TYPE:
         _image_info_type_setup(action, image_data.image->image_name);
         break;
      case PROPERTY_IMAGE_CONTROL_COMPRESSION:
         elm_object_text_set(action->control,
                             edje_image_compression[image_data.image->comp_type]);
         break;
      case PROPERTY_IMAGE_CONTROL_QUALITY:
         if (image_data.image->comp_type == EDJE_EDIT_IMAGE_COMP_LOSSY)
           {
              str = eina_stringshare_printf("%i", image_data.image->quality);
              elm_object_text_set(action->control, str);
              eina_stringshare_del(str);
           }
         else
           elm_object_text_set(action->control, EMPTY_VALUE);
         break;
      case PROPERTY_IMAGE_CONTROL_WIDTH:
         elm_image_object_size_get(image_data.item_preview.action1.control, NULL, &h);
         str = eina_stringshare_printf("%d", h);
         elm_object_text_set(action->control, str);
         eina_stringshare_del(str);
         break;
      case PROPERTY_IMAGE_CONTROL_HEIGHT:
         elm_image_object_size_get(image_data.item_preview.action1.control, &w, NULL);
         str = eina_stringshare_printf("%d", h);
         elm_object_text_set(action->control, str);
         eina_stringshare_del(str);
         break;
      default:
         break;
     }
}

static void
_on_image_selected(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Image_Item *image = (Image_Item *)event_info;

   image_data.image = image;

   _update_cb(NULL, &image_data.item_preview.action1);
   _update_cb(NULL, &image_data.item_name.action1);
   _update_cb(NULL, &image_data.item_type.action1);
   _update_cb(NULL, &image_data.item_location.action1);
   _update_cb(NULL, &image_data.item_image_compression.action1);
   _update_cb(NULL, &image_data.item_image_quality.action1);
   _update_cb(NULL, &image_data.item_width.action1);
   _update_cb(NULL, &image_data.item_height.action1);
}

void
property_image_manager_init()
{
   image_data.item_preview.name = eina_stringshare_add(_("preview"));
   image_data.item_preview.action1.control_type = PROPERTY_CONTROL_IMAGE_PREVIEW;
   image_data.item_preview.action1.update_cb = _update_cb;
   image_data.item_preview.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_PREVIEW;

   image_data.item_name.name = eina_stringshare_add(_("name"));
   image_data.item_name.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_name.action1.update_cb = _update_cb;
   image_data.item_name.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_NAME;

   image_data.item_location.name = eina_stringshare_add(_("location"));
   image_data.item_location.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_location.action1.update_cb = _update_cb;
   image_data.item_location.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_LOCATION;

   image_data.item_type.name = eina_stringshare_add(_("type"));
   image_data.item_type.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_type.action1.update_cb = _update_cb;
   image_data.item_type.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_TYPE;

   image_data.item_image_compression.name = eina_stringshare_add(_("image compression"));
   image_data.item_image_compression.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_image_compression.action1.update_cb = _update_cb;
   image_data.item_image_compression.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_COMPRESSION;

   image_data.item_image_quality.name = eina_stringshare_add(_("image quality"));
   image_data.item_image_quality.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_image_quality.action1.update_cb = _update_cb;
   image_data.item_image_quality.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_QUALITY;

   image_data.item_width.name = eina_stringshare_add(_("width"));
   image_data.item_width.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_width.action1.update_cb = _update_cb;
   image_data.item_width.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_WIDTH;

   image_data.item_height.name = eina_stringshare_add(_("height"));
   image_data.item_height.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_height.action1.update_cb = _update_cb;
   image_data.item_height.action1.type.attribute_image = PROPERTY_IMAGE_CONTROL_HEIGHT;

   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_SELECTED, _on_image_selected, NULL);
}

Eina_List *
property_image_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &image_data.item_preview);
   items = eina_list_append(items, &image_data.item_name);
   items = eina_list_append(items, &image_data.item_location);
   items = eina_list_append(items, &image_data.item_type);
   items = eina_list_append(items, &image_data.item_image_compression);
   items = eina_list_append(items, &image_data.item_image_quality);
   items = eina_list_append(items, &image_data.item_width);
   items = eina_list_append(items, &image_data.item_height);

   return items;
}
