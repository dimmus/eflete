#include "live_view_prop.h"

#define PROP_DATA "prop_data"

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

Eina_Bool
live_view_property_style_set(Evas_Object *property __UNUSED__,
                             Evas_Object *object __UNUSED__,
                             Style *style __UNUSED__,
                             const char *widget __UNUSED__)
{

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
live_view_property_style_unset(Evas_Object *property __UNUSED__)
{

   return true;
}
