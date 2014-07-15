#include "live_view_prop.h"

#define PROP_DATA "prop_data"
#define ITEM "item"
#define PART_NAME "part_name"

#define PROP_DATA_GET(ret) \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   if (!pd) \
     { \
        ERR("Can not show group data, because object[%p] is not a property object", property); \
        return ret; \
     }

struct _Prop_Data
{
   Evas_Object *live_object;
   Evas_Object *visual;
   Evas_Object *scale_spinner;
   double current_scale;
   const char *widget;
   Style *style;
   struct {
      Evas_Object *frame;
      Evas_Object *swallows;
   } prop_swallow;
   struct {
      Evas_Object *frame;
      Evas_Object *texts;
   } prop_text;
};
typedef struct _Prop_Data Prop_Data;

static void
_on_zoom_change(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   pd->current_scale = elm_spinner_value_get(obj) / 100;
   if (pd->live_object)
      elm_object_scale_set(pd->live_object, pd->current_scale);
}

static void
_on_all_swallow_check(void *data,
                      Evas_Object *obj,
                      void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *item = NULL;
   Eina_List *part_list = NULL, *part = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   part_list = elm_box_children_get(pd->prop_swallow.swallows);
   if (!part_list) return;

   EINA_LIST_FOREACH(part_list, part, item)
     {
        check = elm_object_part_content_get(item, "info");
        elm_check_state_set(check, elm_check_state_get(obj));
        evas_object_smart_callback_call(check, "changed", NULL);
     }

   eina_list_free(part_list);
}

static void
_on_swallow_check(void *data,
           Evas_Object *obj,
           void *ei __UNUSED__)
{
   Evas_Object *rect = NULL;

   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     {
        rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, 136, 24, 242, 255);
        elm_object_part_content_set(object, part_name, rect);
     }
   else
     {
        rect = elm_object_part_content_unset(object, part_name);
        evas_object_del(rect);
     }
}

static void
_on_all_text_check(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *item = NULL;
   Eina_List *part_list = NULL, *part = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   part_list = elm_box_children_get(pd->prop_text.texts);
   if (!part_list) return;

   EINA_LIST_FOREACH(part_list, part, item)
     {
        check = elm_object_part_content_get(item, "info");
        elm_check_state_set(check, elm_check_state_get(obj));
        evas_object_smart_callback_call(check, "changed", NULL);
     }

   eina_list_free(part_list);
}

static void
_on_text_check(void *data,
               Evas_Object *obj,
               void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     elm_object_part_text_set(object, part_name, "Look at it! This is absolutely and totally text");
   else
     elm_object_part_text_set(object, part_name, NULL);
}

Eina_Bool
live_view_property_style_set(Evas_Object *property,
                             Evas_Object *object,
                             Style *style,
                             const char *widget)
{
   Evas_Object *prop_box, *spinner, *check;
   Evas_Object *item;
   const char *part_name;
   Eina_List *part_list = NULL, *part = NULL;
   Edje_Part_Type part_type;

   if ((!property) || (!object) || (!style) || (!widget))
     return false;
   PROP_DATA_GET(false)

   evas_object_show(property);

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_ON);

   pd->widget = widget;
   pd->live_object = object;

   prop_box = elm_object_content_get(pd->visual);

   if (!pd->scale_spinner)
     {
        item = elm_layout_add(prop_box);
        evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0);
        elm_layout_file_set(item, EFLETE_EDJ, "eflete/prop/container/live_view_spinner");
        elm_object_part_text_set(item, "eflete.text.start", "Scale: ");
        evas_object_show(item);
        SPINNER_ADD(item, spinner, 1, 500, 1, true, "eflete/live_view");
        elm_spinner_label_format_set(spinner, "%3.0f%%");
        evas_object_smart_callback_add(spinner, "changed", _on_zoom_change, pd);
        pd->scale_spinner = item;
        elm_object_part_content_set(item, "eflete.content", spinner);
        evas_object_data_set(item, ITEM, spinner);
     }
   spinner = evas_object_data_get(pd->scale_spinner, ITEM);
   elm_spinner_value_set(spinner, 100);
   pd->current_scale = 1.0;
   elm_object_scale_set(pd->live_object, pd->current_scale);
   elm_box_pack_start(prop_box, pd->scale_spinner);
   evas_object_show(pd->scale_spinner);

   /* Swallows UI setting*/
   if (!pd->prop_swallow.swallows)
     {
        FRAME_ADD(property, pd->prop_swallow.frame, true, _("Swallows"));
        elm_object_style_set(pd->prop_swallow.frame, "eflete/live_view");

        CHECK_ADD(prop_box, check, "eflete/live_view");
        elm_object_part_content_set(pd->prop_swallow.frame, "elm.swallow.check", check);

        BOX_ADD(pd->prop_swallow.frame, pd->prop_swallow.swallows, false, false)
        elm_box_align_set(pd->prop_swallow.swallows, 0.5, 0.0);
        elm_object_content_set(pd->prop_swallow.frame, pd->prop_swallow.swallows);

        evas_object_smart_callback_add(check, "changed", _on_all_swallow_check, pd);
     }
   elm_box_pack_end(prop_box, pd->prop_swallow.frame);
   evas_object_show(pd->prop_swallow.frame);
   evas_object_show(pd->prop_swallow.swallows);

   /* Texts UI setting*/
   if (!pd->prop_text.texts)
     {
        FRAME_ADD(property, pd->prop_text.frame, true, _("Texts"));
        elm_object_style_set(pd->prop_text.frame, "eflete/live_view");

        CHECK_ADD(prop_box, check, "eflete/live_view");
        elm_object_part_content_set(pd->prop_text.frame, "elm.swallow.check", check);

        BOX_ADD(pd->prop_text.frame, pd->prop_text.texts, false, false)
        elm_box_align_set(pd->prop_text.texts, 0.5, 0.0);
        elm_object_content_set(pd->prop_text.frame, pd->prop_text.texts);

        evas_object_smart_callback_add(check, "changed", _on_all_text_check, pd);
     }

   elm_box_pack_end(prop_box, pd->prop_text.frame);
   evas_object_show(pd->prop_text.frame);
   evas_object_show(pd->prop_text.texts);

   /* setting all swallows with rectangles */
   part_list = edje_edit_parts_list_get(style->obj);

   EINA_LIST_FOREACH(part_list, part, part_name)
     {
        part_type = edje_edit_part_type_get(style->obj, part_name);

        if (part_type ==  EDJE_PART_TYPE_SWALLOW)
          {
             ITEM_ADD(pd->prop_swallow.swallows, item, eina_stringshare_add(part_name), "eflete/property/item/live_view");
             CHECK_ADD(item, check, "eflete/live_view");

             evas_object_smart_callback_add(check, "changed", _on_swallow_check, pd->live_object);
             evas_object_data_set(check, PART_NAME, part_name);

             elm_object_part_content_set(item, "info", check);
             elm_box_pack_end(pd->prop_swallow.swallows, item);
             evas_object_show(item);
          }
        else if ((part_type ==  EDJE_PART_TYPE_TEXT) ||
                 (part_type ==  EDJE_PART_TYPE_TEXTBLOCK))
          {
             ITEM_ADD(pd->prop_text.texts, item, eina_stringshare_add(part_name), "eflete/property/item/live_view");
             CHECK_ADD(item, check, "eflete/live_view");

             evas_object_smart_callback_add(check, "changed", _on_text_check, pd->live_object);
             evas_object_data_set(check, PART_NAME, part_name);

             elm_object_part_content_set(item, "info", check);
             elm_box_pack_end(pd->prop_text.texts, item);
             evas_object_show(item);
          }
     }
   edje_edit_string_list_free(part_list);

   return true;
}

Evas_Object *
live_view_property_add(Evas_Object *parent)
{
   Evas_Object *box, *scroller, *_bg;
   Prop_Data *pd;

   if (!parent) return NULL;

   pd = mem_calloc(1, sizeof(Prop_Data));

   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   _bg = elm_bg_add(parent);
   elm_bg_file_set(_bg, EFLETE_IMG_PATH"section-item-bg.png", NULL);
   elm_object_part_content_set(scroller, "elm.swallow.background", _bg);
   evas_object_show(_bg);
   evas_object_show(scroller);
   evas_object_show(box);

   pd->visual = scroller;

   evas_object_data_set(scroller, PROP_DATA, pd);

   return scroller;
}

Eina_Bool
live_view_property_style_unset(Evas_Object *property)
{
   Evas_Object *prop_box, *item = NULL, *check = NULL;
   Eina_List *part_list = NULL, *part = NULL;

   if (!property) return false;
   PROP_DATA_GET(false)

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd->scale_spinner);
   evas_object_hide(pd->scale_spinner);

   /* Swallows Clear */
   elm_box_unpack(prop_box, pd->prop_swallow.frame);
   evas_object_hide(pd->prop_swallow.frame);
   part_list = elm_box_children_get(pd->prop_swallow.swallows);

   elm_box_unpack_all(pd->prop_swallow.swallows);

   EINA_LIST_FOREACH(part_list, part, item)
     {
        check = elm_object_part_content_unset(item, "info");
        evas_object_smart_callback_del_full(check, "changed", _on_swallow_check, pd->live_object);
        evas_object_data_del(check, PART_NAME);
        evas_object_del(check);
        evas_object_del(item);
     }
   eina_list_free(part_list);

   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");
   elm_check_state_set(check, false);

   /* Texts Clear */
   elm_box_unpack(prop_box, pd->prop_text.frame);
   evas_object_hide(pd->prop_text.frame);
   part_list = elm_box_children_get(pd->prop_text.texts);
   elm_box_unpack_all(pd->prop_text.texts);

   EINA_LIST_FOREACH(part_list, part, item)
     {
        check = elm_object_part_content_unset(item, "info");
        evas_object_smart_callback_del_full(check, "changed", _on_text_check, pd->live_object);
        evas_object_data_del(check, PART_NAME);
        evas_object_del(check);
        evas_object_del(item);
     }
   eina_list_free(part_list);

   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");
   elm_check_state_set(check, false);

   pd->live_object = NULL;

   return true;
}
