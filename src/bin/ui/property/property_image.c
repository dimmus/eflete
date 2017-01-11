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
#include "project_manager2.h"
#include "property.h"
#include "property_private.h"

#define EMPTY_VALUE " - "

#define IMAGE_MASK(TYPE) (1u << TYPE)
#define IMAGE_NORMAL_TYPE IMAGE_MASK(SINGLE_IMAGE)
#define IMAGE_SET_TYPE IMAGE_MASK(IMAGE_SET)
#define IMAGE_SET_IMAGE_TYPE IMAGE_MASK(IMAGE_SET_ITEM)

typedef struct {
   Image_Item *img;
   Property_Attribute items[PROPERTY_IMAGE_ITEM_LAST];
} Property_Image_Data;

/* accroding to Edje_Edit.h */
static const char *edje_image_compression[] = { "RAW",
                                                "USER",
                                                "COMP",
                                                "LOSSY",
                                                "LOSSY_ETC1",
                                                "LOSSY_ETC2",
                                                NULL};

static Property_Image_Data image_pd;

struct _Property_Image_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Image_Update_Info Property_Image_Update_Info;

static Property_Image_Update_Info attribute_map[ATTRIBUTE_IMAGE_ITEM_LAST];

static void
_image_info_type_setup(Property_Action *action, const char *image_name)
{
   char buf[BUFF_MAX];

   assert(image_name != NULL);

   if (image_pd.img->type == IMAGE_SET)
     {
        elm_object_text_set(action->control, _("Set of images"));
        return;
     }

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

static Eina_Bool
_update_cb(Property_Attribute *pa __UNUSED__, Property_Action *action)
{
   Eina_Stringshare *str;


   switch (action->type.attribute_image)
     {
      case ATTRIBUTE_IMAGE_ITEM_NAME:
         elm_object_text_set(action->control, image_pd.img->image_name);
         break;
      case ATTRIBUTE_IMAGE_ITEM_TYPE:
         _image_info_type_setup(action, image_pd.img->image_name);
         break;
      case ATTRIBUTE_IMAGE_ITEM_LOCATION:
         if (image_pd.img->comp_type != EDJE_EDIT_IMAGE_COMP_USER)
           {
              str = eina_stringshare_printf("edje/images/%i", image_pd.img->id);
              elm_object_text_set(action->control, str);
              eina_stringshare_del(str);
           }
         else
           elm_object_text_set(action->control, image_pd.img->image_name);
         break;
      case ATTRIBUTE_IMAGE_ITEM_COMPRESSION:
         elm_object_text_set(action->control,
                             edje_image_compression[image_pd.img->comp_type]);
         break;
      case ATTRIBUTE_IMAGE_ITEM_QUALITY:
         if (image_pd.img->comp_type == EDJE_EDIT_IMAGE_COMP_LOSSY)
           {
              str = eina_stringshare_printf("%i", image_pd.img->quality);
              elm_object_text_set(action->control, str);
              eina_stringshare_del(str);
           }
         else
           elm_object_text_set(action->control, EMPTY_VALUE);
         break;
      case ATTRIBUTE_IMAGE_ITEM_WIDTH:
         str = eina_stringshare_printf("%d", image_pd.img->width);
         elm_object_text_set(action->control, str);
         eina_stringshare_del(str);
         break;
      case ATTRIBUTE_IMAGE_ITEM_HEIGHT:
         str = eina_stringshare_printf("%d", image_pd.img->height);
         elm_object_text_set(action->control, str);
         eina_stringshare_del(str);
         break;
      case ATTRIBUTE_IMAGE_ITEM_BORDER_L:
         elm_spinner_value_set(action->control, image_pd.img->border_l);
         break;
      case ATTRIBUTE_IMAGE_ITEM_BORDER_R:
         elm_spinner_value_set(action->control, image_pd.img->border_r);
         break;
      case ATTRIBUTE_IMAGE_ITEM_BORDER_T:
         elm_spinner_value_set(action->control, image_pd.img->border_t);
         break;
      case ATTRIBUTE_IMAGE_ITEM_BORDER_B:
         elm_spinner_value_set(action->control, image_pd.img->border_b);
         break;
      case ATTRIBUTE_IMAGE_ITEM_MIN_W:
         elm_spinner_value_set(action->control, image_pd.img->min_w);
         break;
      case ATTRIBUTE_IMAGE_ITEM_MIN_H:
         elm_spinner_value_set(action->control, image_pd.img->min_h);
         break;
      case ATTRIBUTE_IMAGE_ITEM_MAX_W:
         elm_spinner_value_set(action->control, image_pd.img->max_w);
         break;
      case ATTRIBUTE_IMAGE_ITEM_MAX_H:
         elm_spinner_value_set(action->control, image_pd.img->max_h);
         break;
      case ATTRIBUTE_IMAGE_ITEM_BORDER_SCALE:
         elm_spinner_value_set(action->control, image_pd.img->border_scale);
         break;
      default:
         CRIT("Failed to update attribute [%s]", action->name ? action->name : "unknown");
         abort();
         break;
     }
   return true;
}

static void
_on_image_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Property_Data *pd = (Property_Data *)data;
   Image_Item *image = (Image_Item *)event_info;

   image_pd.img = image;
   if (!image) return;

   GENLIST_FILTER_APPLY(pd->genlist);
   property_item_update_recursively(&image_pd.items[PROPERTY_IMAGE_ITEM_INFO_TITLE]);
   property_item_update_recursively(&image_pd.items[PROPERTY_IMAGE_ITEM_IMAGE_PROPERTY_TITLE]);
   property_item_update_recursively(&image_pd.items[PROPERTY_IMAGE_ITEM_SET_IMAGE_PROPERTY_TITLE]);
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_image)
     {
      case ATTRIBUTE_IMAGE_ITEM_NAME:
      case ATTRIBUTE_IMAGE_ITEM_TYPE:
      case ATTRIBUTE_IMAGE_ITEM_LOCATION:
      case ATTRIBUTE_IMAGE_ITEM_COMPRESSION:
      case ATTRIBUTE_IMAGE_ITEM_QUALITY:
      case ATTRIBUTE_IMAGE_ITEM_WIDTH:
      case ATTRIBUTE_IMAGE_ITEM_HEIGHT:
         break;
      case ATTRIBUTE_IMAGE_ITEM_BORDER_L:
      case ATTRIBUTE_IMAGE_ITEM_BORDER_R:
      case ATTRIBUTE_IMAGE_ITEM_BORDER_T:
      case ATTRIBUTE_IMAGE_ITEM_BORDER_B:
      case ATTRIBUTE_IMAGE_ITEM_MIN_W:
      case ATTRIBUTE_IMAGE_ITEM_MIN_H:
      case ATTRIBUTE_IMAGE_ITEM_MAX_W:
      case ATTRIBUTE_IMAGE_ITEM_MAX_H:
      case ATTRIBUTE_IMAGE_ITEM_BORDER_SCALE:
         elm_spinner_value_set(action->control, 0);
         elm_object_disabled_set(action->control, true);
         break;
      case ATTRIBUTE_IMAGE_ITEM_LAST:
         break;
      default:
         CRIT("Failed to init attribute [%s]", action->name ? action->name : "unknown");
         abort();
         break;
     }
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type, Attribute_Image_Item attribute,
                 const char *tooltip)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->tooltip = eina_stringshare_add(tooltip);
   action->control_type = control_type;
   action->type.attribute_image = attribute;
   action->update_cb = _update_cb;
   action->init_cb = _init_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Image_Item attribute,
         const char *tooltip)
{
   _action_internal(&pa->action1, name, units, control_type, attribute, tooltip);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Image_Item attribute,
         const char *tooltip)
{
   _action_internal(&pa->action2, name, units, control_type, attribute, tooltip);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action2;
}

static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &image_pd.items[TYPE]);
   switch (pa->type.image_item)
     {
      case PROPERTY_IMAGE_ITEM_INFO_TITLE:
         APPEND(PROPERTY_IMAGE_ITEM_NAME);
         APPEND(PROPERTY_IMAGE_ITEM_TYPE);
         break;
      case PROPERTY_IMAGE_ITEM_IMAGE_PROPERTY_TITLE:
         APPEND(PROPERTY_IMAGE_ITEM_LOCATION);
         APPEND(PROPERTY_IMAGE_ITEM_COMPRESSION);
         APPEND(PROPERTY_IMAGE_ITEM_QUALITY);
         APPEND(PROPERTY_IMAGE_ITEM_WIDTH);
         APPEND(PROPERTY_IMAGE_ITEM_HEIGHT);
         break;
      case PROPERTY_IMAGE_ITEM_SET_IMAGE_PROPERTY_TITLE:
         APPEND(PROPERTY_IMAGE_ITEM_BORDER_H);
         APPEND(PROPERTY_IMAGE_ITEM_BORDER_V);
         APPEND(PROPERTY_IMAGE_ITEM_MIN);
         APPEND(PROPERTY_IMAGE_ITEM_MAX);
         APPEND(PROPERTY_IMAGE_ITEM_BORDER_SCALE);
         break;
      default:
         CRIT("items callbase not found for %s", pa->name);
         abort();
     }
#undef APPEND
   return items;
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   if (image_pd.img)
     {
        if (pa->filter_data.image_types & IMAGE_MASK(image_pd.img->type))
          return true;
     }
   return false;
}

static void
_init_items()
{
   Property_Image_Item it;
#define IT image_pd.items[it]
   for (it = 0; it < PROPERTY_IMAGE_ITEM_LAST; it++)
     {
        IT.type.image_item = it;
        IT.filter_cb = _filter_cb;

        IT.filter_data.image_types = ~0u;
        switch(it)
          {
           case PROPERTY_IMAGE_ITEM_INFO_TITLE:
              IT.name = _("Item info");
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_NAME:
              IT.name = _("Name");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_NAME, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_TYPE:
              IT.name = _("Type");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_TYPE, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_IMAGE_PROPERTY_TITLE:
              IT.name = _("Image info");
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
            case PROPERTY_IMAGE_ITEM_LOCATION:
              IT.name = _("Location");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_LOCATION, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_COMPRESSION:
              IT.name = _("Image compression");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_COMPRESSION, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_QUALITY:
              IT.name = _("Image quality");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_QUALITY, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_WIDTH:
              IT.name = _("Width");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_WIDTH, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_HEIGHT:
              IT.name = _("Height");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_IMAGE_ITEM_HEIGHT, NULL);
              IT.filter_data.image_types = IMAGE_NORMAL_TYPE | IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_SET_IMAGE_PROPERTY_TITLE:
              IT.name = _("Set image info");
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.image_types = IMAGE_SET_IMAGE_TYPE;
              break;
            case PROPERTY_IMAGE_ITEM_BORDER_H:
              IT.name = _("Border");
              _action1(&IT, _("left:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_BORDER_L, NULL);
              _action2(&IT, _("right:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_BORDER_R, NULL);
              IT.filter_data.image_types = IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_BORDER_V:
              IT.name = "";
              _action1(&IT, _("top:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_BORDER_T, NULL);
              _action2(&IT, _("bottom:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_BORDER_B, NULL);
              IT.filter_data.image_types = IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_MIN:
              IT.name = _("Minimum size");
              _action1(&IT, _("width:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_MIN_W, NULL);
              _action2(&IT, _("height:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_MIN_H, NULL);
              IT.filter_data.image_types = IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_MAX:
              IT.name = _("Maximum size");
              _action1(&IT, _("width:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_MAX_W, NULL);
              _action2(&IT, _("height:"), _("px"), PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_MAX_H, NULL);
              IT.filter_data.image_types = IMAGE_SET_IMAGE_TYPE;
              break;
           case PROPERTY_IMAGE_ITEM_BORDER_SCALE:
              IT.name = _("Border scale");
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SPINNER, ATTRIBUTE_IMAGE_ITEM_BORDER_SCALE, NULL);
              IT.filter_data.image_types = IMAGE_SET_IMAGE_TYPE;
              break;
           default:
              CRIT("Failed to init item with type [%d]", it);
              abort();
              break;
          }
     }
#undef IT
}

void
property_image_manager_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();

   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_SELECTED, _on_image_selected, pd);
}

Eina_List *
property_image_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &image_pd.items[PROPERTY_IMAGE_ITEM_INFO_TITLE]);
   items = eina_list_append(items, &image_pd.items[PROPERTY_IMAGE_ITEM_IMAGE_PROPERTY_TITLE]);
   items = eina_list_append(items, &image_pd.items[PROPERTY_IMAGE_ITEM_SET_IMAGE_PROPERTY_TITLE]);

   return items;
}
