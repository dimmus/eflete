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
#include "ewe_widget_tabs.h"

#define MY_CLASS EWE_TABS_CLASS

#define MY_CLASS_NAME "Ewe_Tabs"
#define MY_CLASS_NAME_LEGACY "ewe_tabs"

static const char SIG_ITEM_ACTIVATED[] = "ewe,tabs,item,activated";
static const char SIG_ITEM_DEACTIVATED[] = "ewe,tabs,item,deactivated";

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_ITEM_ACTIVATED, ""},
   {SIG_ITEM_DEACTIVATED, ""},
   {NULL, NULL}
};

static void
_ewe_tabs_layout_style_set(Ewe_Tabs_Smart_Data *sd)
{
   Eina_Stringshare *group_name = NULL;
   group_name = eina_stringshare_printf("ewe/tabs/%s/%s",
                                        sd->horizontal ? "horizontal" : "vertical",
                                        sd->style);
   elm_layout_file_set(sd->layout, EWE_THEME, group_name);
   eina_stringshare_del(group_name);
}

EOLIAN static void
_ewe_tabs_evas_object_smart_add(Eo *obj,
                                Ewe_Tabs_Smart_Data *sd)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   eo_do_super(obj, MY_CLASS, evas_obj_smart_add());

   elm_widget_sub_object_parent_add(obj);

   sd->layout = elm_layout_add(obj);
   evas_object_smart_member_add(sd->layout, obj);

   sd->style = eina_stringshare_add("default");
   _ewe_tabs_layout_style_set(sd);

   sd->items_box = elm_box_add(obj);
   evas_object_smart_member_add(sd->items_box, obj);
   elm_layout_content_set(sd->layout, "tabs_area", sd->items_box);
   elm_box_horizontal_set(sd->items_box, sd->horizontal);
   elm_box_align_set(sd->items_box, 0.0, 0.5);
   sd->items = NULL;

   TODO("add correct style handling")
   /*
   if (!elm_layout_theme_set(obj, "label", "base", elm_widget_style_get(obj)))
     CRI("Failed to set layout!");
   */

   evas_object_smart_changed(obj);
}

EAPI Evas_Object *
ewe_tabs_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = eo_add(MY_CLASS, parent);

   return obj;
}

EOLIAN static void
_ewe_tabs_eo_base_constructor(Eo *obj, Ewe_Tabs_Smart_Data *sd)
{
   sd->obj = obj;
   sd->horizontal = EINA_TRUE;
   eo_do_super(obj, MY_CLASS, eo_constructor());
   eo_do(obj,
         evas_obj_type_set(MY_CLASS_NAME_LEGACY),
         evas_obj_smart_callbacks_descriptions_set(_smart_callbacks));
}

EOLIAN static void
_ewe_tabs_class_constructor(Eo_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

static void
_head_click_cb(void *data,
               Evas_Object *obj EINA_UNUSED,
               const char *emission EINA_UNUSED,
               const char *source EINA_UNUSED)
{
   Ewe_Tabs_Item *it = (Ewe_Tabs_Item *)data;
   ewe_tabs_active_item_set(it->owner, it);
}

static Ewe_Tabs_Item *
_item_add_internal(Evas_Object           *obj,
                   Ewe_Tabs_Smart_Data   *sd EINA_UNUSED,
                   Eina_Stringshare      *title,
                   Eina_Stringshare      *style)
{
   Eina_Strbuf *buf = eina_strbuf_new();

   Ewe_Tabs_Item *ret = malloc(sizeof(Ewe_Tabs_Item));
   ret->owner = obj;
   ret->disabled = EINA_FALSE;
   if (!style) style = "default";
   ret->style = style;
   ret->head = elm_layout_add(obj); TODO("find out howto replace this elm_layout with edje_object")
   evas_object_smart_member_add(ret->head, obj);
   if (sd->horizontal)
     eina_strbuf_append_printf(buf, "ewe/tabs/item/head/horizontal/%s", style);
   else
     eina_strbuf_append_printf(buf, "ewe/tabs/item/head/vertical/%s", style);
   elm_layout_file_set(ret->head, EWE_THEME, eina_strbuf_string_get(buf));

   if (!title) title = "";
   ret->title = eina_stringshare_add(title);
   elm_layout_text_set(ret->head, "ewe.text", title);

   eina_strbuf_reset(buf);
   ret->content = elm_layout_add(obj);
   if (sd->horizontal)
     eina_strbuf_append_printf(buf, "ewe/tabs/item/content/horizontal/%s", style);
   else
     eina_strbuf_append_printf(buf, "ewe/tabs/item/content/vertical/%s", style);
   elm_layout_file_set(ret->content, EWE_THEME, eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);

   elm_layout_signal_callback_add(ret->head, "mouse,clicked,1", "events", _head_click_cb, ret);

   evas_object_show(ret->head);
   return ret;
}

EOLIAN static Ewe_Tabs_Item *
_ewe_tabs_item_append(Eo                    *obj,
                      Ewe_Tabs_Smart_Data   *sd,
                      Ewe_Tabs_Item         *item,
                      Eina_Stringshare      *title,
                      Eina_Stringshare      *style)
{
   Ewe_Tabs_Item *ret = NULL;

   if (!item)
     {
        ret = _item_add_internal(obj, sd, title, style);
        sd->items = eina_list_append(sd->items, ret);
        elm_box_pack_end(sd->items_box, ret->head);
     }
   else
     {
        if (item->owner != obj) return NULL;
        ret = _item_add_internal(obj, sd, title, style);
        sd->items = eina_list_append_relative(sd->items, ret, item);
        elm_box_pack_after(sd->items_box, ret->head, item->head);
     }

   if (!sd->active_item) ewe_tabs_active_item_set(obj, ret);
   return ret;
}

EOLIAN static Ewe_Tabs_Item *
_ewe_tabs_item_prepend(Eo                    *obj,
                       Ewe_Tabs_Smart_Data   *sd,
                       Ewe_Tabs_Item         *item,
                       Eina_Stringshare      *title,
                       Eina_Stringshare      *style)
{
   Ewe_Tabs_Item *ret = NULL;

   if (!item)
     {
        ret = _item_add_internal(obj, sd, title, style);
        sd->items = eina_list_prepend(sd->items, ret);
        elm_box_pack_start(sd->items_box, ret->head);
     }
   else
     {
        if (item->owner != obj) return NULL;
        ret = _item_add_internal(obj, sd, title, style);
        sd->items = eina_list_prepend_relative(sd->items, ret, item);
        elm_box_pack_before(sd->items_box, ret->head, item->head);
     }

   if (!sd->active_item) ewe_tabs_active_item_set(obj, ret);
   return ret;
}

static void
_item_data_free(Ewe_Tabs_Item *item)
{
   if (!item) return;
   evas_object_del(item->head);
   evas_object_del(item->content);
   eina_stringshare_del(item->title);
   free(item);
}

EOLIAN static Eina_Bool
_ewe_tabs_item_del(Eo                    *obj,
                   Ewe_Tabs_Smart_Data   *sd,
                   Ewe_Tabs_Item         *item)
{
   Eina_List * l;
   if ((!item) || (item->owner != obj)) return EINA_FALSE;
   if (item == sd->active_item)
     {
        l = eina_list_data_find_list(sd->items, item);
        if (l)
          {
             if (l->prev)
               ewe_tabs_active_item_set(obj, eina_list_data_get(l->prev));
             else
               ewe_tabs_active_item_set(obj, eina_list_data_get(l->next));
          }
     }
   sd->items = eina_list_remove(sd->items, item);
   _item_data_free(item);
   return EINA_TRUE;
}

EOLIAN static void
_ewe_tabs_evas_object_smart_del(Eo *obj EINA_UNUSED,
                                Ewe_Tabs_Smart_Data *sd)
{
   Ewe_Tabs_Item *it;
   EINA_LIST_FREE(sd->items, it)
     {
        _item_data_free(it);
     }
   evas_object_del(sd->layout);
   eina_stringshare_del(sd->style);
}

EOLIAN static const Eina_List *
_ewe_tabs_items_list_get(Eo *obj EINA_UNUSED,
                         Ewe_Tabs_Smart_Data *sd)
{
   return sd->items;
}

EOLIAN static Ewe_Tabs_Item *
_ewe_tabs_active_item_get(Eo *obj EINA_UNUSED,
                          Ewe_Tabs_Smart_Data *sd)
{
   return sd->active_item;
}

EOLIAN static Eina_Bool
_ewe_tabs_active_item_set(Eo                  *obj,
                          Ewe_Tabs_Smart_Data *sd,
                          Ewe_Tabs_Item       *item)
{
   if ((!item) || (item->owner != obj)) return EINA_FALSE;
   if (sd->active_item)
     {
         if (sd->active_item == item)
           return EINA_TRUE;
         evas_object_smart_callback_call(obj, SIG_ITEM_DEACTIVATED, (void *)sd->active_item);
         evas_object_hide(sd->active_item->content);
         elm_layout_signal_emit(sd->active_item->head, "ewe,tabs,item,deactivate", "ewe");
         sd->active_item->content = elm_layout_content_unset(sd->layout, "content_area");
     }

   elm_layout_signal_emit(item->head, "ewe,tabs,item,activate", "ewe");
   elm_layout_content_set(sd->layout, "content_area", item->content);
   sd->active_item = item;
   evas_object_smart_callback_call(obj, SIG_ITEM_ACTIVATED, (void *)sd->active_item);
   evas_object_show(sd->active_item->content);
   evas_object_raise(sd->items_box);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_tabs_item_content_set(Eo                   *obj,
                           Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                           Ewe_Tabs_Item        *item,
                           Evas_Object          *content)
{
   if ((!item) || (item->owner != obj) || (!content)) return EINA_FALSE;
   return elm_layout_content_set(item->content, "ewe.content", content);
}

EOLIAN static const Evas_Object *
_ewe_tabs_item_content_get(Eo                   *obj,
                             Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                             Ewe_Tabs_Item        *item)
{
   if ((!item) || (item->owner != obj)) return NULL;
   return elm_layout_content_get(item->content, "ewe.content");
}


EOLIAN static Evas_Object *
_ewe_tabs_item_content_unset(Eo                   *obj,
                             Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                             Ewe_Tabs_Item        *item)
{
   if ((!item) || (item->owner != obj)) return NULL;
   return elm_layout_content_unset(item->content, "ewe.content");
}

EOLIAN static Eina_Bool
_ewe_tabs_item_icon_set(Eo *obj,
                        Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                        Ewe_Tabs_Item        *item,
                        Evas_Object          *icon)
{
   Eina_Bool ret = EINA_FALSE;
   if ((!item) || (item->owner != obj) || (!icon)) return EINA_FALSE;
   ret = elm_layout_content_set(item->head, "ewe.icon", icon);
   elm_layout_signal_emit(item->head, "ewe,tabs,item,icon,show", "ewe");
   return ret;;
}

EOLIAN static Evas_Object *
_ewe_tabs_item_icon_unset(Eo *obj,
                          Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                          Ewe_Tabs_Item        *item)
{
   if ((!item) || (item->owner != obj)) return NULL;
   elm_layout_signal_emit(item->head, "ewe,tabs,item,icon,hide", "ewe");
   return elm_layout_content_unset(item->head, "ewe.icon");
}

EOLIAN static Eina_Bool
_ewe_tabs_item_button_set(Eo *obj,
                          Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                          Ewe_Tabs_Item        *item,
                          Evas_Object          *button)
{
   Eina_Bool ret = EINA_FALSE;
   if ((!item) || (item->owner != obj) || (!button)) return EINA_FALSE;
   ret = elm_layout_content_set(item->head, "ewe.swallow.end", button);
   elm_layout_signal_emit(item->head, "ewe,tabs,item,swallow,show", "ewe");
   return ret;;
}

EOLIAN static Evas_Object *
_ewe_tabs_item_button_unset(Eo *obj,
                            Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                            Ewe_Tabs_Item        *item)
{
   if ((!item) || (item->owner != obj)) return NULL;
   elm_layout_signal_emit(item->head, "ewe,tabs,item,swallow,hide", "ewe");
   return elm_layout_content_unset(item->head, "ewe.swallow.end");
}

EOLIAN static Eina_Bool
_ewe_tabs_item_title_set(Eo                   *obj,
                         Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                         Ewe_Tabs_Item        *item,
                         Eina_Stringshare     *title)
{
   if ((item->owner != obj) || (!title)) return EINA_FALSE;

   eina_stringshare_del(item->title);
   item->title = eina_stringshare_add(title);
   return elm_layout_text_set(item->head, "ewe.text", title);
}

EOLIAN static Eina_Stringshare *
_ewe_tabs_item_title_get(Eo                   *obj,
                         Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                         Ewe_Tabs_Item  *item)
{
   if ((!item) || (item->owner != obj)) return NULL;
   return item->title;
}

EOLIAN static Eina_Bool
_ewe_tabs_item_disabled_set(Eo                   *obj,
                            Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                            Ewe_Tabs_Item        *item,
                            Eina_Bool            disabled)
{
   if (item->owner != obj) return EINA_FALSE;

   item->disabled = disabled;
   if (disabled)
     {
        elm_layout_signal_emit(item->head, "ewe,tabs,item,disable", "ewe");
        elm_layout_signal_emit(item->content, "ewe,tabs,item,disable", "ewe");
     }
   else
     {
        elm_layout_signal_emit(item->head, "ewe,tabs,item,enable", "ewe");
        elm_layout_signal_emit(item->content, "ewe,tabs,item,enable", "ewe");
     }
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_tabs_item_disabled_get(Eo                   *obj,
                            Ewe_Tabs_Smart_Data  *sd EINA_UNUSED,
                            Ewe_Tabs_Item        *item)
{
   if ((!item) || (item->owner != obj)) return EINA_FALSE;

   return item->disabled;
}

EOLIAN static Eina_Bool
_ewe_tabs_elm_layout_text_set(Eo                   *obj EINA_UNUSED,
                              Ewe_Tabs_Smart_Data  *sd,
                              const char           *part,
                              const char           *text)
{
   Eina_Bool int_ret = EINA_FALSE;

   if (!part || !strcmp(part, "ewe.text"))
     {
        if (text)
          {
             elm_layout_signal_emit(sd->layout, "ewe,state,text,visible", "ewe");
             int_ret = elm_layout_text_set(sd->layout, "ewe.text", text);
          }
        else
          {
             elm_layout_signal_emit(sd->layout, "ewe,state,text,hidden", "ewe");
          }
     }
   else int_ret = elm_layout_text_set(sd->layout, part, text);

   return int_ret;
}

EOLIAN static const char *
_ewe_tabs_elm_layout_text_get(Eo                   *obj EINA_UNUSED,
                              Ewe_Tabs_Smart_Data  *sd,
                              const char           *part)
{
   if (!part) return elm_layout_text_get(sd->layout, "ewe,text");
   else return elm_layout_text_get(sd->layout, part);
}


EOLIAN static Eina_Bool
_ewe_tabs_orient_horizontal_set(Eo                   *obj,
                                Ewe_Tabs_Smart_Data  *sd,
                                Eina_Bool            horizontal)
{
   Ewe_Tabs_Item *it;
   Eina_List *l;
   Evas_Object *icon, *button, *content;
   if (sd->horizontal == horizontal)
     return EINA_FALSE;
   sd->horizontal = horizontal;
   elm_box_horizontal_set(sd->items_box, sd->horizontal);
   _ewe_tabs_layout_style_set(sd);

   EINA_LIST_FOREACH(sd->items, l, it)
     {
        icon = ewe_tabs_item_icon_unset(obj, it);
        button = ewe_tabs_item_button_unset(obj, it);
        content = ewe_tabs_item_content_unset(obj, it);
        Eina_Strbuf *buf = eina_strbuf_new();
        if (sd->horizontal)
          eina_strbuf_append_printf(buf, "ewe/tabs/item/head/horizontal/%s", it->style);
        else
          eina_strbuf_append_printf(buf, "ewe/tabs/item/head/vertical/%s", it->style);
        elm_layout_file_set(it->head, EWE_THEME, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
        if (sd->horizontal)
          eina_strbuf_append_printf(buf, "ewe/tabs/item/content/horizontal/%s", it->style);
        else
          eina_strbuf_append_printf(buf, "ewe/tabs/item/content/vertical/%s", it->style);
        elm_layout_file_set(it->content, EWE_THEME, eina_strbuf_string_get(buf));
        eina_strbuf_free(buf);
        if (ewe_tabs_item_disabled_get(obj, it))
          ewe_tabs_item_disabled_set(obj, it, EINA_TRUE);
        if (it == ewe_tabs_active_item_get(obj))
          {
             sd->active_item = NULL;
             ewe_tabs_active_item_set(obj, it);
          }
        ewe_tabs_item_icon_set(obj, it, icon);
        ewe_tabs_item_button_set(obj, it, button);
        ewe_tabs_item_content_set(obj, it, content);
     }
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_tabs_orient_horizontal_get(Eo                   *obj EINA_UNUSED,
                                Ewe_Tabs_Smart_Data  *sd)
{
   return sd->horizontal;
}

EOLIAN static void
_ewe_tabs_evas_object_smart_resize(Eo                  *obj,
                                   Ewe_Tabs_Smart_Data *sd,
                                   Evas_Coord          w,
                                   Evas_Coord          h)
{
   evas_object_resize(sd->layout, w, h);

   Eina_List *l;
   Ewe_Tabs_Item  *it;
   EINA_LIST_FOREACH(sd->items, l, it)
     {
       elm_layout_sizing_eval(it->head);
     }

   eo_do_super(obj, MY_CLASS, evas_obj_smart_resize(w, h));
}

EOLIAN static Eina_Bool
_ewe_tabs_elm_widget_style_set(Eo                  *obj EINA_UNUSED,
                               Ewe_Tabs_Smart_Data *sd,
                               const char          *style)
{
   if (!sd) return EINA_FALSE;

   if (style)
     eina_stringshare_replace(&(sd->style), style);
   else
     eina_stringshare_replace(&(sd->style), "default");

   _ewe_tabs_layout_style_set(sd);
   return EINA_TRUE;
}

EOLIAN static const char*
_ewe_tabs_elm_widget_style_get(Eo                  *obj EINA_UNUSED,
                               Ewe_Tabs_Smart_Data *sd)
{
   if (!sd) return NULL;

   return sd->style;
}

static void
_ewe_tabs_evas_object_smart_move(Eo                  *obj,
                                 Ewe_Tabs_Smart_Data *sd,
                                 Evas_Coord          x,
                                 Evas_Coord          y)
{
   evas_object_move(sd->layout, x, y);
   eo_do_super(obj, MY_CLASS, evas_obj_smart_move(x, y));
}

static void
_ewe_tabs_evas_object_smart_show(Eo                  *obj EINA_UNUSED,
                                 Ewe_Tabs_Smart_Data *sd)
{
   evas_object_show(sd->layout);
}

static void
_ewe_tabs_evas_object_smart_hide(Eo                  *obj EINA_UNUSED,
                                 Ewe_Tabs_Smart_Data *sd)
{
   evas_object_hide(sd->layout);
}

#include "ewe_tabs.eo.c"
