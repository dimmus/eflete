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

#include "property.h"
#include "property_private.h"
#include "property_common.h"
#include "widget_macro.h"
#include "sound_player.h"
#include "shortcuts.h"

/* hack to disable spinner value changes when scrolling */
static void
_on_spinner_mouse_wheel(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Evas_Event_Mouse_Wheel *mev = event_info;
   mev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
}

/****************** controls creation functions *******************************/
static void
_stop(Property_Data *pd, Property_Attribute *pa, Property_Action *action)
{
   assert(pd != NULL);
   assert(pa != NULL);
   assert(action != NULL);
   assert((action == &pa->action1) || (action == &pa->action2));

   if (pd->started_action)
     {
        assert(pd->started_action == action);
        assert(pd->started_pa == pa);
        if (action->stop_cb)
          {
             DBG("calling stop_cb of %s (%s)", pa->name, (action->name) ? action->name : "unnamed");
             action->stop_cb(pa, action);
          }
        pd->started_pa = NULL;
        pd->started_action = NULL;
     }
}

static void
_start(Property_Data *pd, Property_Attribute *pa, Property_Action *action)
{
   assert(pd != NULL);
   assert(pa != NULL);
   assert(action != NULL);
   assert((action == &pa->action1) || (action == &pa->action2));

   if (pd->started_action)
     _stop(pd, pd->started_pa, pd->started_action);
   if (action->start_cb)
     {
        DBG("calling start_cb of %s (%s)", pa->name, (action->name) ? action->name : "unnamed");
        action->start_cb(pa, action);
     }
   pd->started_pa = pa;
   pd->started_action = action;
}

static void
_change(Property_Data *pd, Property_Attribute *pa, Property_Action *action)
{
   assert(pd != NULL);
   assert(pa != NULL);
   assert(action != NULL);
   assert((action == &pa->action1) || (action == &pa->action2));
   assert(pd->started_action == action);
   assert(pd->started_pa == pa);

   if (action->change_cb)
     {
        DBG("calling change_cb of %s (%s)", pa->name, (action->name) ? action->name : "unnamed");
        action->change_cb(pa, action);
     }
}

static void
_start_cb(void *data,
          Evas_Object *obj,
          void *event_info __UNUSED__)
{
   PROPERTY_DATA_GET(obj);
   Property_Attribute *pa = data;

   assert(pa != NULL);

   if (pa->action1.control == obj)
     {
        /* hack to disable genlist scrolling while dragging spinner */
        if (pa->action1.control_type == PROPERTY_CONTROL_SPINNER)
          {
             shortcuts_object_push(pa->action1.control);
             /* disable wheel scrolling */
             elm_object_scroll_freeze_push(pa->action1.control);
             /* disable page-up/page-down scrolling */
             elm_object_focus_set(pd->genlist, false);
          }
        _start(pd, pa, &pa->action1);
     }
   else
     {
        assert (pa->action2.control == obj);

        if (pa->action2.control_type == PROPERTY_CONTROL_SPINNER)
          {
             shortcuts_object_push(pa->action2.control);
             elm_object_scroll_freeze_push(pa->action2.control);
             elm_object_focus_set(pd->genlist, false);
          }
        _start(pd, pa, &pa->action2);
     }
}

static void
_stop_cb(void *data,
         Evas_Object *obj,
         void *event_info __UNUSED__)
{
   PROPERTY_DATA_GET(obj);
   Property_Attribute *pa = data;

   assert(pa != NULL);
   if (pa->action1.control == obj)
     {
        /* enable scrolling after finishing spinner drag */
        if (pa->action1.control_type == PROPERTY_CONTROL_SPINNER)
          {
             shortcuts_object_check_pop(pa->action1.control);
             elm_object_scroll_freeze_pop(pa->action1.control);
          }
        _stop(pd, pa, &pa->action1);
     }
   else
     {
        assert (pa->action2.control == obj);

        if (pa->action2.control_type == PROPERTY_CONTROL_SPINNER)
          {
             shortcuts_object_check_pop(pa->action2.control);
             elm_object_scroll_freeze_pop(pa->action2.control);
          }
        _stop(pd, pa, &pa->action2);
     }
}

static void
_start_change_stop_cb(void *data,
                      Evas_Object *obj,
                      void *event_info __UNUSED__)
{
   PROPERTY_DATA_GET(obj);
   Property_Attribute *pa = data;

   assert(pa != NULL);
   if (pa->action1.control == obj)
     {
        if (pd->started_action)
          _change(pd, pa, &pa->action1);
        else
          {
             _start(pd, pa, &pa->action1);
             _change(pd, pa, &pa->action1);
             _stop(pd, pa, &pa->action1);
          }
     }
   else
     {
        assert (pa->action2.control == obj);
        if (pd->started_action)
          _change(pd, pa, &pa->action2);
        else
          {
             _start(pd, pa, &pa->action2);
             _change(pd, pa, &pa->action2);
             _stop(pd, pa, &pa->action2);
          }
     }
}

static void
_start_change_cb(void *data,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   PROPERTY_DATA_GET(obj);
   Property_Attribute *pa = data;

   assert(pa != NULL);
   if (pa->action1.control == obj)
     {
        if (pd->started_action)
          _change(pd, pa, &pa->action1);
        else
          {
             _start(pd, pa, &pa->action1);
             _change(pd, pa, &pa->action1);
          }
     }
   else
     {
        assert (pa->action2.control == obj);
        if (pd->started_action)
          _change(pd, pa, &pa->action2);
        else
          {
             _start(pd, pa, &pa->action2);
             _change(pd, pa, &pa->action2);
          }
     }
}

static char *
_combobox_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   Combobox_Item *item = (Combobox_Item *)data;
   return strdup(item->data);
}

static Evas_Object *
_combobox_cc_content_get(void *data, Evas_Object *obj, const char *part)
{
   Combobox_Cc_Item *item = (Combobox_Cc_Item *)data;

   Evas_Object *object = evas_object_rectangle_add(evas_object_evas_get(obj));

   if (!strcmp(part, "swallow.color1"))
     evas_object_color_set(object, item->r1, item->g1, item->b1, item->a1);
   else if (!strcmp(part, "swallow.color2"))
     evas_object_color_set(object, item->r2, item->g2, item->b2, item->a2);
   else if (!strcmp(part, "swallow.color3"))
     evas_object_color_set(object, item->r3, item->g3, item->b3, item->a3);

   evas_object_show(object);

   return object;
}

static void
_combobox_item_del(void *data,
                   Evas_Object *obj __UNUSED__)
{
   Combobox_Item *item = (Combobox_Item *)data;
   eina_stringshare_del(item->data);
   free(item);
}

static void
_combobox_cc_item_del(void *data,
                   Evas_Object *obj __UNUSED__)
{
   Combobox_Cc_Item *item = (Combobox_Cc_Item *)data;
   eina_stringshare_del(item->data);
   free(item);
}

static void
_combobox_item_pressed_cb(void *data __UNUSED__, Evas_Object *obj,
                          void *event_info)
{
   Combobox_Item *item_obj, *item = elm_object_item_data_get(event_info);
   item_obj = evas_object_data_del(obj, "CURRENT_DATA");
   if (item_obj)
     {
        eina_stringshare_del(item_obj->data);
        free(item_obj);
     }

   item_obj = mem_malloc(sizeof(Combobox_Item));
   item_obj->index = item->index;
   item_obj->data = eina_stringshare_add(item->data);

   evas_object_data_set(obj, "CURRENT_DATA", item_obj);

   _start_change_stop_cb(data, obj, event_info);

   elm_combobox_hover_end(obj);
   elm_entry_cursor_end_set(obj);
}

static void
_combobox_expanded_cb(void *data __UNUSED__,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   shortcuts_object_push(obj);
}

static void
_combobox_dismissed_cb(void *data __UNUSED__,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   shortcuts_object_check_pop(obj);
   TODO("remove this dirty hack");
   /* after dissmissing combobox its entry remains focused.
      this makes problems for shortcuts */
   Evas_Object *focused = elm_object_focused_object_get(ap.win);
   if (focused && !strcmp("elm_entry", evas_object_type_get(focused)))
     {
        elm_object_focus_allow_set(focused, false);
        elm_object_focus_set(focused, false);
     }
}

static Evas_Object *
_control_create(Property_Attribute *pa, Property_Action *action, Evas_Object *parent)
{
   Evas_Object *content = NULL;
   Elm_Genlist_Item_Class *itc;

   assert(pa != NULL);
   assert(action != NULL);
   assert(parent != NULL);

   switch (action->control_type)
     {
      case PROPERTY_CONTROL_CHECK:
         CHECK_ADD(parent, content);
         elm_object_style_set(content, "toggle");
         evas_object_smart_callback_add(content, "changed", _start_change_stop_cb, pa);
         break;
      case PROPERTY_CONTROL_BUTTON:
         BUTTON_ADD(parent, content, NULL);
         evas_object_smart_callback_add(content, "clicked", _start_change_stop_cb, pa);
         break;
      case PROPERTY_CONTROL_COMBOBOX:
         COMBOBOX_ADD(parent, content);
         itc = elm_genlist_item_class_new();
         itc->item_style = "default";
         itc->func.text_get = _combobox_text_get;
         itc->func.del = _combobox_item_del;
         evas_object_data_set(content, "COMMON_ITC", itc);
         evas_object_smart_callback_add(content, "item,pressed", _combobox_item_pressed_cb, pa);
         evas_object_smart_callback_add(content, "expanded", _combobox_expanded_cb, pa);
         evas_object_smart_callback_add(content, "dismissed", _combobox_dismissed_cb, pa);
         break;
      case PROPERTY_CONTROL_COMBOBOX_CC:
         COMBOBOX_ADD(parent, content);
         itc = elm_genlist_item_class_new();
         itc->item_style = "default";
         itc->func.text_get = _combobox_text_get;
         itc->func.content_get = _combobox_cc_content_get;
         itc->func.del = _combobox_cc_item_del;
         evas_object_data_set(content, "COMMON_ITC", itc);
         elm_object_style_set(content, "color_class");
         evas_object_smart_callback_add(content, "item,pressed", _combobox_item_pressed_cb, pa);
         evas_object_smart_callback_add(content, "expanded", _combobox_expanded_cb, pa);
         evas_object_smart_callback_add(content, "dismissed", _combobox_dismissed_cb, pa);
         break;
      case PROPERTY_CONTROL_SPINNER:
         SPINNER_ADD(parent, content, 0.0, 9999.0, 1.0, true);
         evas_object_smart_callback_add(content, "spinner,drag,start", _start_cb, pa);
         evas_object_smart_callback_add(content, "spinner,drag,stop", _stop_cb, pa);
         evas_object_smart_callback_add(content, "changed", _start_change_stop_cb, pa);

         evas_object_event_callback_priority_add(content,
                                                 EVAS_CALLBACK_MOUSE_WHEEL,
                                                 EVAS_CALLBACK_PRIORITY_BEFORE,
                                                 _on_spinner_mouse_wheel, NULL);
         break;
      case PROPERTY_CONTROL_ENTRY:
         ENTRY_ADD(parent, content, true);
         evas_object_smart_callback_add(content, "changed,user", _start_change_cb, pa);
         evas_object_smart_callback_add(content, "activated", _stop_cb, pa);
         evas_object_smart_callback_add(content, "unfocused", _stop_cb, pa);
         break;
      case PROPERTY_CONTROL_COLOR:
         content = property_color_control_add(parent);
         evas_object_smart_callback_add(content, "changed", _start_change_cb, pa);
         evas_object_smart_callback_add(content, "dismissed", _stop_cb, pa);
         break;
      case PROPERTY_CONTROL_COLORSEL:
         content = elm_colorselector_add(parent);
         elm_colorselector_mode_set(content, ELM_COLORSELECTOR_ALL);
         TODO("start/stop callbacks for this item type would be incorrect. Add correct one if needed");
         evas_object_smart_callback_add(content, "changed,user", _start_change_stop_cb, pa);
         evas_object_smart_callback_add(content, "color,item,selected", _start_change_stop_cb, pa);

#if HAVE_TIZEN
         evas_object_size_hint_min_set(content, 402, 261);
         elm_colorselector_palette_clear(content);
         elm_colorselector_palette_name_set(content, "eflete_tizen");
         elm_colorselector_palette_color_add(content, 229, 3, 3, 255);
         elm_colorselector_palette_color_add(content, 20, 218, 20, 255);
         elm_colorselector_palette_color_add(content, 25, 46, 201, 255);
         elm_colorselector_palette_color_add(content, 236, 196, 9, 255);
         elm_colorselector_palette_color_add(content, 237, 10, 234, 255);
         elm_colorselector_palette_color_add(content, 26, 234, 217, 255);
         elm_colorselector_palette_color_add(content, 0, 0, 0, 255);
         elm_colorselector_palette_color_add(content, 255, 255, 255, 255);
         elm_colorselector_palette_color_add(content, 200, 200, 200, 255);
#else
         evas_object_size_hint_min_set(content, 200, 270);
#endif
         break;
      case PROPERTY_CONTROL_LABEL:
         content = elm_label_add(parent);
         break;
      case PROPERTY_CONTROL_IMAGE_NORMAL:
         content = property_image_normal_control_add(parent);
         evas_object_smart_callback_add(content, "image,changed", _start_change_stop_cb, pa);
         break;
      case PROPERTY_CONTROL_IMAGE_TWEEN:
         content = property_image_tween_control_add(parent);
         evas_object_smart_callback_add(content, "image,tween,changed", _start_change_stop_cb, pa);
         break;
      case PROPERTY_CONTROL_IMAGE_SELECTOR:
         content = property_image_selector_get(parent);
         evas_object_smart_callback_add(content, "changed,demo,image", _start_change_stop_cb, pa);
         evas_object_show(content);
         break;
      case PROPERTY_CONTROL_NONE:
      case PROPERTY_CONTROL_LAST:
         CRIT("Wrong control type");
         abort();
         return NULL;
      /* do not add default: */
     }

   if (action->tooltip)
     {
        elm_object_tooltip_text_set(content, action->tooltip);
        elm_object_tooltip_move_freeze_push(content);
     }

   return content;
}

/****************** content_get functions *************************************/
static Evas_Object *
_caption_content_get(void *data,
                     Evas_Object *obj,
                     const char *part)
{
   PROPERTY_DATA_GET(obj);
   Eina_Bool res;
   Evas_Object *content;
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert(!strcmp(part, "swallow.icon"));

   if (pa->icon_name != NULL)
     {
        content = elm_image_add(obj);
        res = elm_image_file_set(content, ap.path.theme_edj, pa->icon_name);
        assert(res == true);

        evas_object_data_set(content, PROPERTY_DATA, pd);
        return content;
     }

   return NULL;
}

static Evas_Object *
_1swallow_content_get(void *data,
                      Evas_Object *obj,
                      const char *part)
{
   PROPERTY_DATA_GET(obj);
   Eina_Bool res;
   Evas_Object *content;
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert(!strcmp(part, "swallow.icon") ||
          !strcmp(part, "swallow.action1"));

   if ((pa->icon_name != NULL) && (!strcmp(part, "swallow.icon")))
     {
        content = elm_image_add(obj);
        res = elm_image_file_set(content, ap.path.theme_edj, pa->icon_name);
        assert(res == true);

        evas_object_data_set(content, PROPERTY_DATA, pd);
        return content;
     }
   if ((pa->action1.control_type != PROPERTY_CONTROL_NONE) && (!strcmp(part, "swallow.action1")))
     {
        content = _control_create(pa, &pa->action1, obj);
        pa->action1.control = content;
        if (pa->action1.init_cb != NULL)
          {
             DBG("calling init_cb of %s (%s)", pa->name, (pa->action1.name) ? pa->action1.name : "unnamed");
             pa->action1.init_cb(pa, &pa->action1);
          }

        evas_object_data_set(content, PROPERTY_DATA, pd);
        return content;
     }

   return NULL;
}

static Evas_Object *
_2swallow_content_get(void *data,
                      Evas_Object *obj,
                      const char *part)
{
   PROPERTY_DATA_GET(obj);
   Eina_Bool res;
   Evas_Object *content;
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert(!strcmp(part, "swallow.icon")    ||
          !strcmp(part, "swallow.action1") ||
          !strcmp(part, "swallow.action2"));

   if ((pa->icon_name != NULL) && (!strcmp(part, "swallow.icon")))
     {
        content = elm_image_add(obj);
        res = elm_image_file_set(content, ap.path.theme_edj, pa->icon_name);
        assert(res == true);

        evas_object_data_set(content, PROPERTY_DATA, pd);
        return content;
     }
   if ((pa->action1.control_type != PROPERTY_CONTROL_NONE) && (!strcmp(part, "swallow.action1")))
     {
        content = _control_create(pa, &pa->action1, obj);
        pa->action1.control = content;
        if (pa->action1.init_cb != NULL)
          {
             DBG("calling init_cb of %s (%s)", pa->name, (pa->action1.name) ? pa->action1.name : "unnamed");
             pa->action1.init_cb(pa, &pa->action1);
          }

        evas_object_data_set(content, PROPERTY_DATA, pd);
        return content;
     }
   if ((pa->action2.control_type != PROPERTY_CONTROL_NONE) && (!strcmp(part, "swallow.action2")))
     {
        content = _control_create(pa, &pa->action2, obj);
        pa->action2.control = content;
        if (pa->action2.init_cb != NULL)
          {
             DBG("calling init_cb of %s (%s)", pa->name, (pa->action2.name) ? pa->action2.name : "unnamed");
             pa->action2.init_cb(pa, &pa->action2);
          }

        evas_object_data_set(content, PROPERTY_DATA, pd);
        return content;
     }

   return NULL;
}
/****************** text_get functions ****************************************/
static char *
_caption_text_get(void *data,
                  Evas_Object *obj __UNUSED__,
                  const char *part)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert(!strcmp(part, "text.item.name"));

   if (pa->name != NULL)
     return strdup(pa->name);

   return NULL;
}

static char *
_1swallow_text_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char *part)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert((!strcmp(part, "text.item.name"))    ||
          (!strcmp(part, "text.action1.name")) ||
          (!strcmp(part, "text.action1.units")));

   if ((pa->name != NULL) && (!strcmp(part, "text.item.name")))
     return strdup(pa->name);

   if ((pa->action1.name != NULL) && (!strcmp(part, "text.action1.name")))
     return strdup(pa->action1.name);

   if ((pa->action1.units != NULL) && (!strcmp(part, "text.action1.units")))
     return strdup(pa->action1.units);

   return NULL;
}

static char *
_2swallow_text_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char *part)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert((!strcmp(part, "text.item.name"))     ||
          (!strcmp(part, "text.action1.name"))  ||
          (!strcmp(part, "text.action1.units")) ||
          (!strcmp(part, "text.action2.name"))  ||
          (!strcmp(part, "text.action2.units")));

   if ((pa->name != NULL) && (!strcmp(part, "text.item.name")))
     return strdup(pa->name);

   if ((pa->action1.name != NULL) && (!strcmp(part, "text.action1.name")))
     return strdup(pa->action1.name);

   if ((pa->action1.units != NULL) && (!strcmp(part, "text.action1.units")))
     return strdup(pa->action1.units);

   if ((pa->action2.name != NULL) && (!strcmp(part, "text.action2.name")))
     return strdup(pa->action2.name);

   if ((pa->action2.units != NULL) && (!strcmp(part, "text.action2.units")))
     return strdup(pa->action2.units);

   return NULL;
}

static void
_del(void *data,
     Evas_Object *obj __UNUSED__)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);

   pa->glit = NULL;

   /* pa can be freed in this callback so you shouldn't use it below this line */
   if (pa->del_cb)
     pa->del_cb(pa);
}

static Eina_Bool
_filter(void *data,
        Evas_Object *obj __UNUSED__,
        void *key __UNUSED__)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);

   if (pa->filter_cb)
     return pa->filter_cb(pa);
   return true;
}

static Eina_Bool
_state_get(void *data,
           Evas_Object *obj __UNUSED__,
           const char *state)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);

   if ((pa->default_is) && ((!strcmp(state, "default"))))
     return true;
   if ((!pa->default_is) && ((!strcmp(state, "changed"))))
     return true;
   return false;
}

void
property_common_itc_init(Property_Data *pd)
{
   assert(pd != NULL);

   /* init item classes */
   pd->itc_caption = elm_genlist_item_class_new();
   pd->itc_caption->item_style = "caption";
   pd->itc_caption->func.text_get = _caption_text_get;
   pd->itc_caption->func.content_get = _caption_content_get;
   pd->itc_caption->func.del = _del;
   pd->itc_caption->func.filter_get = _filter;

   pd->itc_1swallow = elm_genlist_item_class_new();
   pd->itc_1swallow->item_style = "1swallow";
   pd->itc_1swallow->func.text_get = _1swallow_text_get;
   pd->itc_1swallow->func.content_get = _1swallow_content_get;
   pd->itc_1swallow->func.del = _del;
   pd->itc_1swallow->func.filter_get = _filter;
   pd->itc_1swallow->func.state_get = _state_get;

   pd->itc_1swallow_wide = elm_genlist_item_class_new();
   pd->itc_1swallow_wide->item_style = "1swallow_wide";
   pd->itc_1swallow_wide->func.text_get = _1swallow_text_get;
   pd->itc_1swallow_wide->func.content_get = _1swallow_content_get;
   pd->itc_1swallow_wide->func.del = _del;
   pd->itc_1swallow_wide->func.filter_get = _filter;
   pd->itc_1swallow_wide->func.state_get = _state_get;

   pd->itc_2swallow = elm_genlist_item_class_new();
   pd->itc_2swallow->item_style = "2swallow";
   pd->itc_2swallow->func.text_get = _2swallow_text_get;
   pd->itc_2swallow->func.content_get = _2swallow_content_get;
   pd->itc_2swallow->func.del = _del;
   pd->itc_2swallow->func.filter_get = _filter;
   pd->itc_2swallow->func.state_get = _state_get;

   /* map control pairs to item classes */
   pd->item_classes[PROPERTY_CONTROL_NONE]           [PROPERTY_CONTROL_NONE]     = pd->itc_caption;

   pd->item_classes[PROPERTY_CONTROL_ENTRY]          [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;
   pd->item_classes[PROPERTY_CONTROL_COMBOBOX]       [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;
   pd->item_classes[PROPERTY_CONTROL_COMBOBOX_CC]    [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;
   pd->item_classes[PROPERTY_CONTROL_COLORSEL]       [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow_wide;
   pd->item_classes[PROPERTY_CONTROL_LABEL]          [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;
   pd->item_classes[PROPERTY_CONTROL_IMAGE_NORMAL]   [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;
   pd->item_classes[PROPERTY_CONTROL_IMAGE_TWEEN]    [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;
   pd->item_classes[PROPERTY_CONTROL_IMAGE_SELECTOR] [PROPERTY_CONTROL_NONE]     = pd->itc_1swallow;

   pd->item_classes[PROPERTY_CONTROL_CHECK]          [PROPERTY_CONTROL_NONE]     = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_CHECK]          [PROPERTY_CONTROL_CHECK]    = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_CHECK]          [PROPERTY_CONTROL_SPINNER]  = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_CHECK]          [PROPERTY_CONTROL_COLOR]    = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_SPINNER]        [PROPERTY_CONTROL_NONE]     = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_SPINNER]        [PROPERTY_CONTROL_SPINNER]  = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_COLOR]          [PROPERTY_CONTROL_NONE]     = pd->itc_2swallow;
   pd->item_classes[PROPERTY_CONTROL_COLOR]          [PROPERTY_CONTROL_COLOR]    = pd->itc_2swallow;
}
