#include "live_view_prop.h"

#define PROP_DATA "prop_data"
#define ITEM "item"

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

Eina_Bool
live_view_property_style_set(Evas_Object *property,
                             Evas_Object *object,
                             Style *style __UNUSED__,
                             const char *widget)
{
   Evas_Object *prop_box, *spinner;
   Evas_Object *item;

   if ((!property) || (!object)) return false;
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

   evas_object_data_set(parent, PROP_DATA, pd);

   return scroller;
}

Eina_Bool
live_view_property_style_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   if (!property) return false;
   PROP_DATA_GET(false)

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd->scale_spinner);
   evas_object_hide(pd->scale_spinner);

   return true;
}
