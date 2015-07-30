/* Elementary Widgets Extension
* Copyright (C) 2014 Samsung Electronics.
*
* This file is part of Elementary Widgets Extension.
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

#include "ewe_private.h"
#include "ewe_widget_statusbar.h"

#define MY_CLASS EWE_STATUSBAR_CLASS

#define MY_CLASS_NAME "Ewe_Statusbar"
#define MY_CLASS_NAME_LEGACY "ewe_statusbar"

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {NULL, NULL}
};

/*--------------internal helpers functions------------*/
static void
_item_click_cb(void *data,
               Evas_Object *obj EINA_UNUSED,
               const char *emission EINA_UNUSED,
               const char *source EINA_UNUSED)
{
   Ewe_Statusbar_Item *item = (Ewe_Statusbar_Item *)data;

   if (!item->func) return;
   item->func(item->cb_data, item->parent, item);
}



static Ewe_Statusbar_Item *
_item_new(Evas_Object *parent, Ewe_Statusbar_Items_Type type, Evas_Object *content,
          Evas_Smart_Cb func, void *cb_data)
{
   Ewe_Statusbar_Item *item = NULL;

   EWE_STATUSBAR_DATA_GET_OR_RETURN_VAL(parent, sd, NULL);

   item = malloc(sizeof(Ewe_Statusbar_Item));
   if (!item) return NULL;
   item->parent = parent;
   item->markup = edje_object_add(evas_object_evas_get(item->parent));
   item->type = type;
   item->disabled = EINA_FALSE;
   item->width = DEFAULT_ITEM_WIDTH;
   item->id = 0;
   item->func = func;
   item->cb_data = cb_data;

   switch (type)
     {
      case EWE_STATUSBAR_ITEM_TYPE_OBJECT:
        {
           item->content = content;
           edje_object_file_set(item->markup, EWE_THEME,
                                "ewe/statusbar/item/default");
           edje_object_part_swallow(item->markup, "ewe.swallow.content",
                                    item->content);
           edje_object_signal_callback_add(item->markup, "statusbar,item,clicked",
                                           "ewe", _item_click_cb, item);
        }
      break;
      case EWE_STATUSBAR_ITEM_TYPE_SEPARATOR:
        {
            item->content = NULL;
            edje_object_file_set(item->markup, EWE_THEME,
                                 "ewe/statusbar/item/separator");
            item->width = SEPARATOR_WIDTH;
            item->func = NULL;
            item->cb_data = NULL;
        }
      break;
      default:
        {
           evas_object_del(item->markup);
           free(item);
           return NULL;
        }
     }
   evas_object_size_hint_min_set(item->markup, item->width, sd->height);
   evas_object_show(item->markup);

   return item;
}

static void
_item_free(Ewe_Statusbar_Item *item)
{
   evas_object_del(item->content);
   evas_object_del(item->markup);
   free(item);
}

static void
_items_id_reorder(Eina_List *items)
{
   Ewe_Statusbar_Item *item = NULL;
   Eina_List *l = NULL;
   int i = 0;

   EINA_LIST_FOREACH(items, l, item)
      item->id = i++;
}

/*--------internal realisation API---------*/
EOLIAN static Eina_Bool
_ewe_statusbar_clear(Eo *obj EINA_UNUSED, Ewe_Statusbar_Smart_Data *sd)
{
   Ewe_Statusbar_Item *item = NULL;
   int count = 0;

   if (sd->items)
     EINA_LIST_FREE(sd->items, item)
       {
          evas_object_smart_member_del(item->markup);
          evas_object_del(item->content);
          evas_object_del(item->markup);
          free(item);
       }

   count = eina_list_count(sd->items);

   if (count > 0) return EINA_FALSE;

   return EINA_TRUE;
}

EOLIAN static Ewe_Statusbar_Item *
_ewe_statusbar_item_append(Eo *obj EINA_UNUSED,
                           Ewe_Statusbar_Smart_Data *sd,
                           Evas_Object *content,
                           Ewe_Statusbar_Items_Type type,
                           Evas_Smart_Cb func, void *func_data)
{
   Ewe_Statusbar_Item *item = NULL;

   item = _item_new(obj, type, content, func, func_data);

   if(item)
     {
        evas_object_smart_member_add(item->markup, obj);
        evas_object_box_append(sd->box, item->markup);
        item->id = eina_list_count(sd->items) + 1;
        sd->items = eina_list_append(sd->items, item);
     }
   return item;
}

EOLIAN static Ewe_Statusbar_Item *
_ewe_statusbar_item_prepend(Eo *obj EINA_UNUSED,
                            Ewe_Statusbar_Smart_Data *sd,
                            Evas_Object *content,
                            Ewe_Statusbar_Items_Type type,
                            Evas_Smart_Cb func, void *func_data)
{
   Ewe_Statusbar_Item *item = NULL;

   item = _item_new(obj, type, content, func, func_data);

   if(item)
     {
        evas_object_smart_member_add(item->markup, obj);
        evas_object_box_prepend(sd->box, item->markup);
        sd->items = eina_list_prepend(sd->items, item);
        _items_id_reorder(sd->items);
     }
   return item;
}

EOLIAN static Eina_Bool
_ewe_statusbar_item_insert_before(Eo * obj,
                                  Ewe_Statusbar_Smart_Data *sd,
                                  Ewe_Statusbar_Item *item,
                                  Ewe_Statusbar_Item *before)
{
   if  ((!item) || (!before) || (item->parent != obj) || (before->parent != obj))
     return EINA_FALSE;
   if (item == before)
     return EINA_FALSE;

   if (!evas_object_box_remove(sd->box, item->markup)) return EINA_FALSE;
   sd->items = eina_list_remove(sd->items, item);

   sd->items = eina_list_prepend_relative(sd->items, item, before);
   evas_object_box_insert_before(sd->box, item->markup, before->markup);
   _items_id_reorder(sd->items);

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_statusbar_item_insert_after(Eo * obj,
                                 Ewe_Statusbar_Smart_Data *sd,
                                 Ewe_Statusbar_Item *item,
                                 Ewe_Statusbar_Item *after)

{
   if ((!item) || (!after) || (item->parent != obj) || (after->parent != obj))
     return EINA_FALSE;
   if (item == after)
     return EINA_FALSE;

   if (!evas_object_box_remove(sd->box, item->markup)) return EINA_FALSE;
   sd->items = eina_list_remove(sd->items, item);

   sd->items = eina_list_append_relative(sd->items, item, after);
   evas_object_box_insert_after(sd->box, item->markup, after->markup);
   _items_id_reorder(sd->items);

   return EINA_TRUE;
}


EOLIAN static Eina_Bool
_ewe_statusbar_items_swap(Eo *obj EINA_UNUSED,
                          Ewe_Statusbar_Smart_Data *sd EINA_UNUSED,
                          Ewe_Statusbar_Item *item_first EINA_UNUSED,
                          Ewe_Statusbar_Item *item_second EINA_UNUSED)
{
   return EINA_FALSE;
}

EOLIAN static const Eina_List *
_ewe_statusbar_items_list_get(Eo *obj EINA_UNUSED, Ewe_Statusbar_Smart_Data *sd)
{
   return sd->items;
}

EOLIAN static int
_ewe_statusbar_items_padding_get(Eo *obj EINA_UNUSED,
                                 Ewe_Statusbar_Smart_Data *sd EINA_UNUSED)
{
   return -2;
}

EOLIAN static void
_ewe_statusbar_items_padding_set(Eo *obj EINA_UNUSED,
                                 Ewe_Statusbar_Smart_Data *sd EINA_UNUSED,
                                 int padding EINA_UNUSED)
{
   return;
}

EOLIAN static Ewe_Statusbar_Items_Align
_ewe_statusbar_items_align_get(Eo *obj EINA_UNUSED,
                               Ewe_Statusbar_Smart_Data *sd EINA_UNUSED)
{
   return EWE_STATUSBAR_ITEMS_ALIGN_LAST;
}

EOLIAN static void
_ewe_statusbar_items_align_set(Eo *obj EINA_UNUSED,
                               Ewe_Statusbar_Smart_Data *sd EINA_UNUSED,
                               Ewe_Statusbar_Items_Align align EINA_UNUSED)
{
   return;
}

EOLIAN static Eina_Bool
_ewe_statusbar_item_remove(Eo *obj EINA_UNUSED,
                           Ewe_Statusbar_Smart_Data *sd,
                           Ewe_Statusbar_Item *item)
{
   if (!item)
     {
        ERR("Cannot delete NULL item");
        return EINA_FALSE;
     }

   if (!evas_object_box_remove(sd->box, item->markup))
     {
        ERR("Fail to delete item from container");
        return EINA_FALSE;
     }

   sd->items = eina_list_remove(sd->items, item);
   _item_free(item);
   _items_id_reorder(sd->items);

   return EINA_TRUE;
}

/*---------Smart functions ----------*/
EOLIAN static void
_ewe_statusbar_evas_object_smart_add(Eo *obj, Ewe_Statusbar_Smart_Data *priv)
{
   eo_do_super(obj, MY_CLASS, evas_obj_smart_add());
   elm_widget_sub_object_parent_add(obj);

   priv->obj = obj;
   priv->items = NULL;

   priv->markup = edje_object_add(evas_object_evas_get(obj));
   edje_object_file_set(priv->markup, EWE_THEME, "ewe/statusbar/base/default");
   evas_object_smart_member_add(priv->markup, obj);

   edje_object_size_max_get(priv->markup, NULL, &priv->height);

   priv->box = evas_object_box_add(evas_object_evas_get(obj));
   evas_object_box_align_set(priv->box, 0.0, 0.5);
   evas_object_smart_member_add(priv->box, obj);
   edje_object_part_swallow(priv->markup, "ewe.swallow.content", priv->box);
   evas_object_show(priv->box);
}

EOLIAN static void
_ewe_statusbar_evas_object_smart_del(Eo *obj, Ewe_Statusbar_Smart_Data *sd)
{
   _ewe_statusbar_clear(obj, sd);

   evas_object_smart_member_del(sd->box);
   evas_object_del(sd->box);

   evas_object_smart_member_del(sd->markup);
   evas_object_del(sd->markup);

}

EOLIAN static void
_ewe_statusbar_evas_object_smart_show(Eo *obj EINA_UNUSED, Ewe_Statusbar_Smart_Data *sd)
{
   evas_object_show(sd->markup);
}

EOLIAN static void
_ewe_statusbar_evas_object_smart_hide(Eo *obj EINA_UNUSED, Ewe_Statusbar_Smart_Data *sd)
{
   evas_object_hide(sd->markup);
}

EOLIAN static void
_ewe_statusbar_evas_object_smart_move(Eo *obj,
                                      Ewe_Statusbar_Smart_Data *sd,
                                      Evas_Coord x,
                                      Evas_Coord y)
{
   eo_do_super(obj, MY_CLASS, evas_obj_smart_move(x, y));

   evas_object_move(sd->markup, x, y);
}

EOLIAN static void
_ewe_statusbar_evas_object_smart_resize(Eo *obj,
                                        Ewe_Statusbar_Smart_Data *sd,
                                        Evas_Coord w,
                                        Evas_Coord h EINA_UNUSED)
{
   eo_do_super(obj, MY_CLASS, evas_obj_smart_resize(w, sd->height));
   evas_object_resize(sd->markup, w, sd->height);
}

/*---------Legacy functions -------------*/
EAPI Evas_Object *
ewe_statusbar_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = eo_add(MY_CLASS, parent);
   return obj;
}
/* ------Item API--------*/
EAPI int
ewe_statusbar_item_id_get(Ewe_Statusbar_Item *item EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, -1);

   return -1;
}

EAPI int
ewe_statusbar_item_width_get(Ewe_Statusbar_Item *item)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, -2);

   return item->width;
}

EAPI Eina_Bool
ewe_statusbar_item_width_set(Ewe_Statusbar_Item *item, int width)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, EINA_FALSE);
   EWE_STATUSBAR_DATA_GET_OR_RETURN_VAL(item->parent, sd, EINA_FALSE);

   if (item->type == EWE_STATUSBAR_ITEM_TYPE_SEPARATOR) return EINA_FALSE;

   if (width < -1) return EINA_FALSE;

   item->width = width;
   evas_object_size_hint_min_set(item->markup, item->width, sd->height);
   return EINA_TRUE;
}

EAPI char *
ewe_statusbar_item_label_get(Ewe_Statusbar_Item *item)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, NULL);

   return (char *)edje_object_part_text_get(item->markup, "ewe.text");
}

EAPI Eina_Bool
ewe_statusbar_item_label_set(Ewe_Statusbar_Item *item, const char *label)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(label, EINA_FALSE);

   return edje_object_part_text_set(item->markup, "ewe.text", label);
}

EAPI Evas_Object *
ewe_statusbar_item_content_get(Ewe_Statusbar_Item *item)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, NULL);

   return item->content;
}

EAPI Eina_Bool
ewe_statusbar_item_content_set(Ewe_Statusbar_Item *item, Evas_Object *content)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(content, EINA_FALSE);

   if (item->type == EWE_STATUSBAR_ITEM_TYPE_SEPARATOR) return EINA_FALSE;

   if (item->content)
     {
        edje_object_part_unswallow(item->markup, item->content);
        evas_object_del(item->content);
     }

   item->content = content;
   return edje_object_part_swallow(item->markup, "ewe.swallow.content", content);
}

EAPI Evas_Object *
ewe_statusbar_item_content_unset(Ewe_Statusbar_Item *item)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, NULL);
   Evas_Object *content = NULL;

   content = item->content;
   edje_object_part_unswallow(item->markup, item->content);
   item->content = NULL;

   return content;
}

EAPI Evas_Object *
ewe_statusbar_item_statusbar_get(Ewe_Statusbar_Item *item EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(item, NULL);

   return NULL;
}

/* -------EO operations---------- */

EOLIAN static Eo*
_ewe_statusbar_eo_base_constructor(Eo *obj, Ewe_Statusbar_Smart_Data *sd)
{
   obj = eo_do_super_ret(obj, MY_CLASS, obj, eo_constructor());
   sd->obj = obj;
   eo_do(obj,
         evas_obj_type_set(MY_CLASS_NAME_LEGACY),
         evas_obj_smart_callbacks_descriptions_set(_smart_callbacks));
   return obj;
}

EOLIAN void
_ewe_statusbar_class_constructor(Eo_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

#include "ewe_statusbar.eo.c"
